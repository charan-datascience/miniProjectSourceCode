// Enhanced Bank-Account Management System
// Features: CRUD, Search by name, Sort & display, Transaction history log
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ─── Constants ───────────────────────────────────────────────────────────────
#define MAX_ACCOUNTS   100
#define HISTORY_FILE   "transactions.log"
#define DATA_FILE      "credit.dat"
#define ACCOUNTS_FILE  "accounts.txt"

// ─── Structures ──────────────────────────────────────────────────────────────
struct clientData {
    unsigned int acctNum;
    char         lastName[15];
    char         firstName[10];
    double       balance;
};

// ─── Prototypes ──────────────────────────────────────────────────────────────
unsigned int enterChoice(void);
void         printBanner(void);
void         printSeparator(void);

// Original features
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

// New features
void searchByName(FILE *fPtr);
void sortAndDisplay(FILE *fPtr);
void viewTransactionLog(void);
void logTransaction(unsigned int acctNum, const char *type,
                    double amount, double newBalance);

// ─── Helpers ─────────────────────────────────────────────────────────────────
void printBanner(void) {
    printf("\n");
    printf("  ╔══════════════════════════════════════════╗\n");
    printf("  ║       BANK ACCOUNT MANAGEMENT SYSTEM     ║\n");
    printf("  ║              Credit Manager v2.0         ║\n");
    printf("  ╚══════════════════════════════════════════╝\n");
    printf("\n");
}

void printSeparator(void) {
    printf("  ──────────────────────────────────────────────\n");
}

// ─── Main ────────────────────────────────────────────────────────────────────
int main(void) {
    FILE        *cfPtr;
    unsigned int choice;

    printBanner();

    if ((cfPtr = fopen(DATA_FILE, "rb+")) == NULL) {
        // File doesn't exist yet — create it with 100 blank records
        printf("  [INFO] No data file found. Creating new '%s'...\n\n", DATA_FILE);
        if ((cfPtr = fopen(DATA_FILE, "wb+")) == NULL) {
            printf("  [ERROR] Could not create data file.\n");
            exit(-1);
        }
        struct clientData blank = {0, "", "", 0.0};
        for (int i = 0; i < MAX_ACCOUNTS; i++)
            fwrite(&blank, sizeof(struct clientData), 1, cfPtr);
        rewind(cfPtr);
    }

    while ((choice = enterChoice()) != 8) {
        switch (choice) {
        case 1: textFile(cfPtr);       break;
        case 2: updateRecord(cfPtr);   break;
        case 3: newRecord(cfPtr);      break;
        case 4: deleteRecord(cfPtr);   break;
        case 5: searchByName(cfPtr);   break;  // NEW
        case 6: sortAndDisplay(cfPtr); break;  // NEW
        case 7: viewTransactionLog();  break;  // NEW
        default:
            printf("\n  [!] Invalid choice. Please select 1-8.\n");
            break;
        }
    }

    fclose(cfPtr);
    printf("\n  Goodbye! All changes saved.\n\n");
    return 0;
}

// ─── Menu ────────────────────────────────────────────────────────────────────
unsigned int enterChoice(void) {
    unsigned int menuChoice;
    printf("\n");
    printSeparator();
    printf("  MENU\n");
    printSeparator();
    printf("  1. Export accounts to text file (accounts.txt)\n");
    printf("  2. Update an account balance\n");
    printf("  3. Add a new account\n");
    printf("  4. Delete an account\n");
    printf("  5. Search accounts by name        [NEW]\n");
    printf("  6. Sort & display all accounts    [NEW]\n");
    printf("  7. View transaction history log   [NEW]\n");
    printf("  8. Exit\n");
    printSeparator();
    printf("  Choice: ");
    scanf("%u", &menuChoice);
    return menuChoice;
}

// ─── Feature 1: Export to Text File ─────────────────────────────────────────
void textFile(FILE *readPtr) {
    FILE              *writePtr;
    int                result;
    struct clientData  client = {0, "", "", 0.0};
    int                count  = 0;

    if ((writePtr = fopen(ACCOUNTS_FILE, "w")) == NULL) {
        printf("\n  [ERROR] Could not open '%s'.\n", ACCOUNTS_FILE);
        return;
    }

    rewind(readPtr);
    fprintf(writePtr, "%-6s  %-15s  %-10s  %12s\n",
            "Acct", "Last Name", "First Name", "Balance");
    fprintf(writePtr,
            "------  ---------------  ----------  ------------\n");

    while (!feof(readPtr)) {
        result = fread(&client, sizeof(struct clientData), 1, readPtr);
        if (result != 0 && client.acctNum != 0) {
            fprintf(writePtr, "%-6d  %-15s  %-10s  %12.2f\n",
                    client.acctNum, client.lastName,
                    client.firstName, client.balance);
            count++;
        }
    }

    fprintf(writePtr, "\nTotal accounts: %d\n", count);
    fclose(writePtr);
    printf("\n  [OK] %d account(s) written to '%s'.\n", count, ACCOUNTS_FILE);
}

// ─── Feature 2: Update Record ────────────────────────────────────────────────
void updateRecord(FILE *fPtr) {
    unsigned int      account;
    double            transaction;
    struct clientData client = {0, "", "", 0.0};

    printf("\n  Enter account to update (1-%d): ", MAX_ACCOUNTS);
    if (scanf("%u", &account) != 1 || account < 1 || account > MAX_ACCOUNTS) {
        printf("  [ERROR] Invalid account number.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("  [ERROR] Account #%u does not exist.\n", account);
        return;
    }

    printf("\n  %-6s  %-15s  %-10s  %12s\n",
           "Acct", "Last Name", "First Name", "Balance");
    printSeparator();
    printf("  %-6u  %-15s  %-10s  %12.2f\n",
           client.acctNum, client.lastName,
           client.firstName, client.balance);

    printf("\n  Enter charge (+) or payment (-): ");
    if (scanf("%lf", &transaction) != 1) {
        printf("  [ERROR] Invalid amount.\n");
        return;
    }

    double oldBalance  = client.balance;
    client.balance    += transaction;

    printf("\n  Updated balance: %.2f\n", client.balance);

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    // Log the transaction
    logTransaction(client.acctNum,
                   transaction >= 0 ? "CHARGE" : "PAYMENT",
                   transaction, client.balance);

    printf("  [OK] Account #%u updated (%.2f -> %.2f).\n",
           client.acctNum, oldBalance, client.balance);
}

// ─── Feature 3: New Record ───────────────────────────────────────────────────
void newRecord(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
    unsigned int      accountNum;

    printf("\n  Enter new account number (1-%d): ", MAX_ACCOUNTS);
    if (scanf("%u", &accountNum) != 1 ||
        accountNum < 1 || accountNum > MAX_ACCOUNTS) {
        printf("  [ERROR] Invalid account number.\n");
        return;
    }

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0) {
        printf("  [ERROR] Account #%u already exists (%s %s).\n",
               client.acctNum, client.firstName, client.lastName);
        return;
    }

    printf("  Enter last name, first name, opening balance\n  ? ");
    if (scanf("%14s%9s%lf",
              client.lastName, client.firstName, &client.balance) != 3) {
        printf("  [ERROR] Invalid input.\n");
        return;
    }

    client.acctNum = accountNum;
    fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    logTransaction(client.acctNum, "OPEN", client.balance, client.balance);

    printf("  [OK] Account #%u created for %s %s (Balance: %.2f).\n",
           client.acctNum, client.firstName, client.lastName, client.balance);
}

// ─── Feature 4: Delete Record ────────────────────────────────────────────────
void deleteRecord(FILE *fPtr) {
    struct clientData client      = {0, "", "", 0.0};
    struct clientData blankClient = {0, "", "", 0.0};
    unsigned int      accountNum;

    printf("\n  Enter account number to delete (1-%d): ", MAX_ACCOUNTS);
    if (scanf("%u", &accountNum) != 1 ||
        accountNum < 1 || accountNum > MAX_ACCOUNTS) {
        printf("  [ERROR] Invalid account number.\n");
        return;
    }

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("  [ERROR] Account #%u does not exist.\n", accountNum);
        return;
    }

    // Confirm deletion
    char confirm;
    printf("  Delete account #%u (%s %s, Balance: %.2f)? [y/N]: ",
           client.acctNum, client.firstName, client.lastName, client.balance);
    scanf(" %c", &confirm);
    if (confirm != 'y' && confirm != 'Y') {
        printf("  [CANCELLED] Account not deleted.\n");
        return;
    }

    logTransaction(client.acctNum, "CLOSE", 0.0, client.balance);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);

    printf("  [OK] Account #%u (%s %s) deleted.\n",
           accountNum, client.firstName, client.lastName);
}

// ─── NEW Feature 5: Search by Name ──────────────────────────────────────────
void searchByName(FILE *fPtr) {
    char              keyword[30];
    struct clientData client;
    int               found = 0;

    printf("\n  Enter last name or first name to search: ");
    scanf("%29s", keyword);

    // Convert keyword to lowercase for case-insensitive match
    char kwLower[30];
    for (int i = 0; keyword[i]; i++)
        kwLower[i] = (char)((keyword[i] >= 'A' && keyword[i] <= 'Z')
                            ? keyword[i] + 32 : keyword[i]);
    kwLower[strlen(keyword)] = '\0';

    rewind(fPtr);
    printf("\n  Search results for \"%s\":\n", keyword);
    printSeparator();
    printf("  %-6s  %-15s  %-10s  %12s\n",
           "Acct", "Last Name", "First Name", "Balance");
    printSeparator();

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum == 0) continue;

        // Convert names to lowercase for comparison
        char lnLower[15], fnLower[10];
        for (int i = 0; client.lastName[i]; i++)
            lnLower[i] = (char)((client.lastName[i] >= 'A' &&
                                  client.lastName[i] <= 'Z')
                                 ? client.lastName[i] + 32
                                 : client.lastName[i]);
        lnLower[strlen(client.lastName)] = '\0';

        for (int i = 0; client.firstName[i]; i++)
            fnLower[i] = (char)((client.firstName[i] >= 'A' &&
                                  client.firstName[i] <= 'Z')
                                 ? client.firstName[i] + 32
                                 : client.firstName[i]);
        fnLower[strlen(client.firstName)] = '\0';

        if (strstr(lnLower, kwLower) || strstr(fnLower, kwLower)) {
            printf("  %-6u  %-15s  %-10s  %12.2f\n",
                   client.acctNum, client.lastName,
                   client.firstName, client.balance);
            found++;
        }
    }

    printSeparator();
    if (found == 0)
        printf("  No accounts found matching \"%s\".\n", keyword);
    else
        printf("  %d account(s) found.\n", found);
}

// ─── NEW Feature 6: Sort & Display All Accounts ──────────────────────────────
void sortAndDisplay(FILE *fPtr) {
    struct clientData accounts[MAX_ACCOUNTS];
    int               count = 0;
    struct clientData client;

    // Load all active records
    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum != 0)
            accounts[count++] = client;
    }

    if (count == 0) {
        printf("\n  [INFO] No accounts found.\n");
        return;
    }

    // Ask sort preference
    printf("\n  Sort by:\n");
    printf("    1. Account number\n");
    printf("    2. Last name (A-Z)\n");
    printf("    3. Balance (high to low)\n");
    printf("    4. Balance (low to high)\n");
    printf("  Choice: ");

    unsigned int sortChoice;
    scanf("%u", &sortChoice);

    // Bubble sort
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int swap = 0;
            switch (sortChoice) {
            case 1:
                swap = accounts[j].acctNum > accounts[j+1].acctNum;
                break;
            case 2:
                swap = strcmp(accounts[j].lastName,
                              accounts[j+1].lastName) > 0;
                break;
            case 3:
                swap = accounts[j].balance < accounts[j+1].balance;
                break;
            case 4:
                swap = accounts[j].balance > accounts[j+1].balance;
                break;
            default:
                swap = accounts[j].acctNum > accounts[j+1].acctNum;
                break;
            }
            if (swap) {
                struct clientData tmp = accounts[j];
                accounts[j]          = accounts[j+1];
                accounts[j+1]        = tmp;
            }
        }
    }

    // Display
    double totalBalance = 0.0;
    printf("\n");
    printSeparator();
    printf("  %-6s  %-15s  %-10s  %12s\n",
           "Acct", "Last Name", "First Name", "Balance");
    printSeparator();
    for (int i = 0; i < count; i++) {
        printf("  %-6u  %-15s  %-10s  %12.2f\n",
               accounts[i].acctNum, accounts[i].lastName,
               accounts[i].firstName, accounts[i].balance);
        totalBalance += accounts[i].balance;
    }
    printSeparator();
    printf("  Total accounts : %d\n", count);
    printf("  Total balance  : %.2f\n", totalBalance);
    printf("  Average balance: %.2f\n", totalBalance / count);
    printSeparator();
}

// ─── NEW Feature 7: View Transaction Log ─────────────────────────────────────
void viewTransactionLog(void) {
    FILE *logPtr;
    char  line[256];
    int   lineCount = 0;

    if ((logPtr = fopen(HISTORY_FILE, "r")) == NULL) {
        printf("\n  [INFO] No transaction history found ('%s' does not exist yet).\n",
               HISTORY_FILE);
        return;
    }

    printf("\n");
    printSeparator();
    printf("  TRANSACTION HISTORY LOG\n");
    printSeparator();
    printf("  %-20s  %-6s  %-8s  %12s  %12s\n",
           "Timestamp", "Acct", "Type", "Amount", "New Balance");
    printSeparator();

    while (fgets(line, sizeof(line), logPtr)) {
        // Remove trailing newline for clean display
        line[strcspn(line, "\n")] = '\0';
        printf("  %s\n", line);
        lineCount++;
    }

    fclose(logPtr);
    printSeparator();

    if (lineCount == 0)
        printf("  [INFO] Log file is empty.\n");
    else
        printf("  %d transaction(s) on record.\n", lineCount);
}

// ─── Helper: Write to Transaction Log ────────────────────────────────────────
void logTransaction(unsigned int acctNum, const char *type,
                    double amount, double newBalance) {
    FILE  *logPtr;
    time_t now = time(NULL);
    char   timestamp[25];

    strftime(timestamp, sizeof(timestamp),
             "%Y-%m-%d %H:%M:%S", localtime(&now));

    if ((logPtr = fopen(HISTORY_FILE, "a")) == NULL) {
        printf("  [WARNING] Could not write to transaction log.\n");
        return;
    }

    fprintf(logPtr, "%-20s  %-6u  %-8s  %12.2f  %12.2f\n",
            timestamp, acctNum, type, amount, newBalance);

    fclose(logPtr);
}
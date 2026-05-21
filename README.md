# Transaction Processing System  
### Mini Project – 24UCS271 Lab

---

## Objective
This mini project focuses on understanding, modifying, and extending a **Transaction Processing System** program. Students are expected to analyze existing code, improve it, and add new functionalities while following good coding practices.

---

## Features Implemented
- Create new account records with account number, last name, first name, and opening balance
- Update account balances with charges or payments
- Delete existing accounts with confirmation prompt
- Export active accounts into a formatted `accounts.txt`
- Search active accounts by first or last name (case-insensitive)
- Sort and display all active accounts by:
  - account number
  - last name
  - balance high-to-low
  - balance low-to-high
- View transaction history from `transactions.log`
- Automatic logging for account open, close, charge, and payment events

---

## Reference Code
Review the given source code before starting the project:

🔗 https://replit.com/@ashokb/Unit5Programs#trans.c

---

## Tasks to be Performed

1. Compile and run the given executable code.
2. Perform your own testing of the program.
3. Add a new account detail and regenerate the `accounts.txt` file.
   - Inspect the contents of the file.
   - Verify whether the output matches your expectations.
4. Identify and fix any logical errors in the program.
5. Add new functionality  
   - Example: Listing all account information.
6. Optimize the program for better performance and efficiency.

---

## Evaluation Criteria

### 1. General (25 Points)
| Activity | Points | Remarks |
|--------|--------|--------|
| Self-effort | 5 | Student’s own assessment of hours spent (no copying or external references) |
| Turnaround Time | 10 | Provide three time slots; earlier demo gets more points |
| Project Demo | 10 | Demo must be done using repl.it |

---

### 2. Comprehension (15 Points)
| Activity | Points |
|--------|--------|
| Domain Knowledge | 5 |
| Added Functionality (Ideas) | 5 |
| Code Comprehension | 5 |

---

### 3. Modification (35 Points)
| Activity | Points |
|--------|--------|
| Code Improvement | 5 |
| Functional Decomposition | 10 |
| Refactoring for Memory Usage | 10 |
| Refactoring for Speed | 10 |

---

### 4. Innovation (45 Points)
| Activity | Points |
|--------|--------|
| New Features / User Stories (Faculty specified) | 5 |
| Requirement to Code Translation | 10 |
| Added Functionality (Simple – Error Handling) | 10 |
| Added Functionality (Advanced) | 20 |

---

## Total Marks
**120 Marks**  
➡ Reduced to **20 Marks** for final evaluation

---

## Rules for Mini Project Implementation

- Maximum of **2 attempts** will be given for project presentation.
- If dissatisfied with the first score, a second attempt is allowed.
- The **best score** out of the two attempts will be considered final.
- Discussion and clarification among students are allowed.
- **Copying or borrowing code is strictly prohibited.**
- Violation of plagiarism rules will result in a **zero (0 / NULL) score**.

---

## Additional Reference
🔗 http://j.mp/transactionUnit5  
🔗 https://htmlpreview.github.io/?https://github.com/kgisl/makesite/blob/master/content/blog/2021-06-23-unit5-mini-project.md.html  
🔗 https://github.com/24UCS271-MiniProject/miniProjectSourceCode/

---

**Date:** 21/05/2026  
**Version:** Credit Manager v2.0
1. Open a terminal in the project folder.
2. Compile using a C compiler, for example:
   ```powershell
   gcc trans.c -o trans.exe
   ```
3. Run the program:
   ```powershell
   .\trans.exe
   ```
4. Use the menu to select operations.

---

## Usage Notes
- The program creates `credit.dat` automatically if it does not exist.
- `accounts.txt` is generated when choosing the export option.
- `transactions.log` is updated automatically for each account operation.
- Accounts are stored in fixed slots from 1 to 100.

---

## New/Enhanced Functionalities
- `Search accounts by name` allows quick lookup using partial or full first/last names.
- `Sort & display all accounts` shows active accounts sorted using the selected criterion.
- `View transaction history log` provides a chronological audit trail of account operations.

---

## Notes for Testing
- Add new accounts and verify they appear in `accounts.txt` after export.
- Perform balance updates and confirm `transactions.log` entries.
- Search for accounts using different case combinations to validate case-insensitive matching.
- Sort accounts and confirm totals/averages displayed are correct.

---

## References
- Original reference code: https://replit.com/@ashokb/Unit5Programs#trans.c

---

**Date:** 21/05/2026
**Version:** Credit Manager v2.0

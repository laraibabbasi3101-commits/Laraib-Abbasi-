#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

class BankAccount {
private:
    int accountNumber;
    string holderName;
    string accountType;
    double balance;

public:
    BankAccount(int accNo, string name, string type, double bal) {
        accountNumber = accNo;
        holderName = name;
        accountType = type;
        balance = bal;
    }

    int getAccountNumber() const { return accountNumber; }
    string getHolderName() const { return holderName; }
    string getAccountType() const { return accountType; }
    double getBalance() const { return balance; }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Amount deposited successfully! New Balance: $" << fixed << setprecision(2) << balance << endl;
        } else {
            cout << "Invalid deposit amount!\n";
        }
    }

    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Withdrawal successful! Remaining Balance: $" << fixed << setprecision(2) << balance << endl;
            return true;
        } else if (amount > balance) {
            cout << "Error: Insufficient funds!\n";
            return false;
        } else {
            cout << "Invalid withdrawal amount!\n";
            return false;
        }
    }

    void displayAccount() const {
        cout << left << setw(15) << accountNumber
             << setw(25) << holderName
             << setw(15) << accountType
             << "$" << fixed << setprecision(2) << setw(12) << balance << endl;
    }
};

class BankSystem {
private:
    vector<BankAccount> accounts;
    const string dataFile = "bank_records.txt";

    void saveToFile() {
        ofstream out(dataFile);
        for (const auto& acc : accounts) {
            out << acc.getAccountNumber() << "|"
                << acc.getHolderName() << "|"
                << acc.getAccountType() << "|"
                << acc.getBalance() << endl;
        }
        out.close();
    }

    void loadFromFile() {
        ifstream in(dataFile);
        if (!in) return;
        accounts.clear();
        string line;
        while (getline(in, line)) {
            size_t p1 = line.find('|');
            size_t p2 = line.find('|', p1 + 1);
            size_t p3 = line.find('|', p2 + 1);
            if (p1 != string::npos && p2 != string::npos && p3 != string::npos) {
                int accNo = stoi(line.substr(0, p1));
                string name = line.substr(p1 + 1, p2 - p1 - 1);
                string type = line.substr(p2 + 1, p3 - p2 - 1);
                double bal = stod(line.substr(p3 + 1));
                accounts.push_back(BankAccount(accNo, name, type, bal));
            }
        }
        in.close();
    }

public:
    BankSystem() { loadFromFile(); }

    void createAccount() {
        int accNo;
        string name, type;
        double initialDeposit;

        cout << "\nEnter New Account Number: ";
        cin >> accNo;
        for (const auto& acc : accounts) {
            if (acc.getAccountNumber() == accNo) {
                cout << "Error: Account Number already exists!\n";
                return;
            }
        }

        cin.ignore();
        cout << "Enter Account Holder Full Name: ";
        getline(cin, name);
        cout << "Enter Account Type (Savings / Current): ";
        getline(cin, type);
        cout << "Enter Initial Deposit Amount ($): ";
        cin >> initialDeposit;

        if (initialDeposit < 0) {
            cout << "Initial deposit cannot be negative!\n";
            return;
        }

        accounts.push_back(BankAccount(accNo, name, type, initialDeposit));
        saveToFile();
        cout << "Bank account created successfully!\n";
    }

    void depositMoney() {
        int accNo;
        double amount;
        cout << "\nEnter Account Number: ";
        cin >> accNo;

        for (auto& acc : accounts) {
            if (acc.getAccountNumber() == accNo) {
                cout << "Enter Amount to Deposit ($): ";
                cin >> amount;
                acc.deposit(amount);
                saveToFile();
                return;
            }
        }
        cout << "Account not found!\n";
    }

    void withdrawMoney() {
        int accNo;
        double amount;
        cout << "\nEnter Account Number: ";
        cin >> accNo;

        for (auto& acc : accounts) {
            if (acc.getAccountNumber() == accNo) {
                cout << "Enter Amount to Withdraw ($): ";
                cin >> amount;
                if (acc.withdraw(amount)) {
                    saveToFile();
                }
                return;
            }
        }
        cout << "Account not found!\n";
    }

    void checkBalance() const {
        int accNo;
        cout << "\nEnter Account Number: ";
        cin >> accNo;

        for (const auto& acc : accounts) {
            if (acc.getAccountNumber() == accNo) {
                cout << "\n--- ACCOUNT DETAILS ---\n";
                cout << "Account No  : " << acc.getAccountNumber() << endl;
                cout << "Holder Name : " << acc.getHolderName() << endl;
                cout << "Account Type: " << acc.getAccountType() << endl;
                cout << "Balance     : $" << fixed << setprecision(2) << acc.getBalance() << endl;
                return;
            }
        }
        cout << "Account not found!\n";
    }

    void viewAllAccounts() const {
        if (accounts.empty()) {
            cout << "\nNo bank accounts available.\n";
            return;
        }
        cout << "\n=====================================================================\n";
        cout << left << setw(15) << "Account No"
             << setw(25) << "Holder Name"
             << setw(15) << "Type"
             << setw(12) << "Balance" << endl;
        cout << "=====================================================================\n";
        for (const auto& acc : accounts) {
            acc.displayAccount();
        }
        cout << "=====================================================================\n";
    }
};

int main() {
    BankSystem bank;
    int choice;

    do {
        cout << "\n=========================================\n";
        cout << "     BANKING & ATM MANAGEMENT SYSTEM     \n";
        cout << "=========================================\n";
        cout << "1. Create New Bank Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Check Account Balance & Details\n";
        cout << "5. View All Accounts (Admin View)\n";
        cout << "6. Exit\n";
        cout << "Enter your choice (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1: bank.createAccount(); break;
            case 2: bank.depositMoney(); break;
            case 3: bank.withdrawMoney(); break;
            case 4: bank.checkBalance(); break;
            case 5: bank.viewAllAccounts(); break;
            case 6: cout << "Thank you for using our Banking System. Goodbye!\n"; break;
            default: cout << "Invalid choice! Please select 1-6.\n";
        }
    } while (choice != 6);

    return 0;
}

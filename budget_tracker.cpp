#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

// Structure to store transaction details
struct Transaction {
    string type; // "Income" or "Expense"
    string category;
    double amount;
};

// Function to display the menu
void showMenu() {
    cout << "\n==============================\n";
    cout << "   PERSONAL FINANCE TRACKER   \n";
    cout << "==============================\n";
    cout << "1. Add Income\n";
    cout << "2. Add Expense\n";
    cout << "3. View Transaction History & Balance\n";
    cout << "4. Save & Exit\n";
    cout << "Enter your choice (1-4): ";
}

int main() {
    vector<Transaction> transactions;
    double balance = 0.0;
    int choice;

    // Optional: Load previous data if file exists
    ifstream inFile("transactions.txt");
    string t, c;
    double amt;
    while (inFile >> t >> c >> amt) {
        transactions.push_back({t, c, amt});
        if (t == "Income") balance += amt;
        else balance -= amt;
    }
    inFile.close();

    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                string category;
                double amount;
                cout << "Enter Income Category (e.g., Salary, PocketMoney): ";
                cin >> category;
                cout << "Enter Amount: ";
                cin >> amount;

                if (amount > 0) {
                    transactions.push_back({"Income", category, amount});
                    balance += amount;
                    cout << "Income added successfully!\n";
                } else {
                    cout << "Invalid amount!\n";
                }
                break;
            }
            case 2: {
                string category;
                double amount;
                cout << "Enter Expense Category (e.g., Food, Transport): ";
                cin >> category;
                cout << "Enter Amount: ";
                cin >> amount;

                if (amount > 0 && amount <= balance) {
                    transactions.push_back({"Expense", category, amount});
                    balance -= amount;
                    cout << "Expense recorded successfully!\n";
                } else if (amount > balance) {
                    cout << "Error: Insufficient balance!\n";
                } else {
                    cout << "Invalid amount!\n";
                }
                break;
            }
            case 3: {
                cout << "\n--- TRANSACTION HISTORY ---\n";
                if (transactions.empty()) {
                    cout << "No transactions recorded yet.\n";
                } else {
                    cout << left << setw(10) << "Type" << setw(15) << "Category" << setw(10) << "Amount" << endl;
                    cout << "------------------------------------\n";
                    for (const auto& t : transactions) {
                        cout << left << setw(10) << t.type << setw(15) << t.category << "$" << setw(10) << t.amount << endl;
                    }
                }
                cout << "------------------------------------\n";
                cout << "Current Balance: $" << balance << "\n";
                break;
            }
            case 4: {
                // Save to file before exiting
                ofstream outFile("transactions.txt");
                for (const auto& t : transactions) {
                    outFile << t.type << " " << t.category << " " << t.amount << endl;
                }
                outFile.close();
                cout << "Data saved successfully. Exiting program. Goodbye!\n";
                break;
            }
            default:
                cout << "Invalid choice! Please choose between 1 and 4.\n";
        }
    } while (choice != 4);

    return 0;
}

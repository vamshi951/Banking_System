#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <ctime>
#include <vector>
using namespace std;

// ─────────────────────────────────────────────
//  Account Class
// ─────────────────────────────────────────────
class Account {
private:
    int accountNumber;
    string holderName;
    string password;
    double balance;
    vector<string> transactionHistory;

    string currentTimestamp() {
        time_t now = time(0);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return string(buf);
    }

public:
    // Default constructor
    Account() : accountNumber(0), holderName(""), password(""), balance(0.0) {}

    // Parameterized constructor
    Account(int accNo, const string& name, const string& pwd, double initialDeposit)
        : accountNumber(accNo), holderName(name), password(pwd), balance(initialDeposit) {
        transactionHistory.push_back(
            "[" + currentTimestamp() + "] Account created. Initial deposit: Rs." +
            to_string(initialDeposit)
        );
    }

    // Getters
    int getAccountNumber() const { return accountNumber; }
    string getHolderName()  const { return holderName; }
    double getBalance()     const { return balance; }

    // Authentication
    bool authenticate(const string& pwd) const {
        return password == pwd;
    }

    // Deposit
    bool deposit(double amount) {
        if (amount <= 0) return false;
        balance += amount;
        transactionHistory.push_back(
            "[" + currentTimestamp() + "] Deposited: Rs." + to_string(amount) +
            " | Balance: Rs." + to_string(balance)
        );
        return true;
    }

    // Withdrawal
    bool withdraw(double amount) {
        if (amount <= 0 || amount > balance) return false;
        balance -= amount;
        transactionHistory.push_back(
            "[" + currentTimestamp() + "] Withdrawn: Rs." + to_string(amount) +
            " | Balance: Rs." + to_string(balance)
        );
        return true;
    }

    // Display account info
    void displayInfo() const {
        cout << "\n╔══════════════════════════════════════╗\n";
        cout << "  Account Number : " << accountNumber       << "\n";
        cout << "  Holder Name    : " << holderName          << "\n";
        cout << fixed << setprecision(2);
        cout << "  Balance        : Rs." << balance          << "\n";
        cout << "╚══════════════════════════════════════╝\n";
    }

    // Display transaction history
    void displayHistory() const {
        cout << "\n--- Transaction History for Account " << accountNumber << " ---\n";
        if (transactionHistory.empty()) {
            cout << "  No transactions found.\n";
        } else {
            for (const auto& record : transactionHistory)
                cout << "  " << record << "\n";
        }
        cout << "------------------------------------------------\n";
    }
};

// ─────────────────────────────────────────────
//  Bank Class
// ─────────────────────────────────────────────
class Bank {
private:
    map<int, Account> accounts;   // STL map: accountNumber -> Account
    int nextAccountNumber;

public:
    Bank() : nextAccountNumber(1001) {}

    // Create a new account
    void createAccount() {
        string name, password;
        double initialDeposit;

        cout << "\n=== Create New Account ===\n";
        cout << "Enter Full Name       : ";
        cin.ignore();
        getline(cin, name);
        cout << "Set Password          : ";
        cin >> password;
        cout << "Initial Deposit (Rs.) : ";
        cin >> initialDeposit;

        if (initialDeposit < 0) {
            cout << "  [!] Initial deposit cannot be negative.\n";
            return;
        }

        Account acc(nextAccountNumber, name, password, initialDeposit);
        accounts[nextAccountNumber] = acc;
        cout << "\n  [✔] Account created successfully!\n";
        cout << "  Your Account Number : " << nextAccountNumber << "\n";
        nextAccountNumber++;
    }

    // Login helper — returns pointer to account or nullptr
    Account* login(int accNo, const string& pwd) {
        auto it = accounts.find(accNo);
        if (it == accounts.end()) return nullptr;
        if (!it->second.authenticate(pwd)) return nullptr;
        return &it->second;
    }

    // Deposit menu
    void depositMenu() {
        int accNo; string pwd; double amount;
        cout << "\n=== Deposit ===\n";
        cout << "Account Number : "; cin >> accNo;
        cout << "Password       : "; cin >> pwd;

        Account* acc = login(accNo, pwd);
        if (!acc) { cout << "  [!] Invalid account number or password.\n"; return; }

        cout << "Amount to Deposit (Rs.): "; cin >> amount;
        if (acc->deposit(amount))
            cout << fixed << setprecision(2)
                 << "  [✔] Deposited Rs." << amount
                 << ". New Balance: Rs." << acc->getBalance() << "\n";
        else
            cout << "  [!] Invalid deposit amount.\n";
    }

    // Withdrawal menu
    void withdrawMenu() {
        int accNo; string pwd; double amount;
        cout << "\n=== Withdraw ===\n";
        cout << "Account Number : "; cin >> accNo;
        cout << "Password       : "; cin >> pwd;

        Account* acc = login(accNo, pwd);
        if (!acc) { cout << "  [!] Invalid account number or password.\n"; return; }

        cout << "Amount to Withdraw (Rs.): "; cin >> amount;
        if (acc->withdraw(amount))
            cout << fixed << setprecision(2)
                 << "  [✔] Withdrawn Rs." << amount
                 << ". New Balance: Rs." << acc->getBalance() << "\n";
        else
            cout << "  [!] Insufficient funds or invalid amount.\n";
    }

    // Balance inquiry
    void balanceInquiry() {
        int accNo; string pwd;
        cout << "\n=== Balance Inquiry ===\n";
        cout << "Account Number : "; cin >> accNo;
        cout << "Password       : "; cin >> pwd;

        Account* acc = login(accNo, pwd);
        if (!acc) { cout << "  [!] Invalid account number or password.\n"; return; }

        acc->displayInfo();
    }

    // Transaction history
    void transactionHistory() {
        int accNo; string pwd;
        cout << "\n=== Transaction History ===\n";
        cout << "Account Number : "; cin >> accNo;
        cout << "Password       : "; cin >> pwd;

        Account* acc = login(accNo, pwd);
        if (!acc) { cout << "  [!] Invalid account number or password.\n"; return; }

        acc->displayHistory();
    }

    // List all accounts (admin view)
    void listAllAccounts() const {
        cout << "\n=== All Accounts ===\n";
        if (accounts.empty()) {
            cout << "  No accounts found.\n";
            return;
        }
        cout << left << setw(15) << "Acc. No."
             << setw(25) << "Holder Name"
             << "Balance (Rs.)\n";
        cout << string(50, '-') << "\n";
        for (const auto& pair : accounts) {
            cout << left << setw(15) << pair.second.getAccountNumber()
                 << setw(25) << pair.second.getHolderName()
                 << fixed << setprecision(2) << pair.second.getBalance() << "\n";
        }
    }
};

// ─────────────────────────────────────────────
//  Main Menu
// ─────────────────────────────────────────────
int main() {
    Bank bank;
    int choice;

    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║      Welcome to C++ Bank System    ║\n";
    cout << "╚════════════════════════════════════╝\n";

    do {
        cout << "\n╔══════════ MAIN MENU ══════════╗\n";
        cout << "║  1. Create Account            ║\n";
        cout << "║  2. Deposit                   ║\n";
        cout << "║  3. Withdraw                  ║\n";
        cout << "║  4. Balance Inquiry           ║\n";
        cout << "║  5. Transaction History       ║\n";
        cout << "║  6. List All Accounts         ║\n";
        cout << "║  0. Exit                      ║\n";
        cout << "╚═══════════════════════════════╝\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: bank.createAccount();      break;
            case 2: bank.depositMenu();        break;
            case 3: bank.withdrawMenu();       break;
            case 4: bank.balanceInquiry();     break;
            case 5: bank.transactionHistory(); break;
            case 6: bank.listAllAccounts();    break;
            case 0: cout << "\n  Goodbye! Thank you for banking with us.\n\n"; break;
            default: cout << "  [!] Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}

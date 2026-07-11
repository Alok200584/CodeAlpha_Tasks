#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <memory>
#include <algorithm>

// ==========================================
// 1. TRANSACTION CLASS
// ==========================================
class Transaction {
private:
    std::string type; // "Deposit", "Withdrawal", or "Transfer"
    double amount;
    std::string details;

public:
    Transaction(std::string t, double amt, std::string det = "") 
        : type(t), amount(amt), details(det) {}

    void displayTransaction() const {
        std::cout << std::left << std::setw(15) << type 
                  << std::setw(12) <<std::fixed << std::setprecision(2) << amount 
                  << details << std::endl;
    }
};

// ==========================================
// 2. ACCOUNT CLASS
// ==========================================
class Account {
private:
    std::string accountNumber;
    double balance;
    std::vector<Transaction> transactionHistory;

public:
    Account(std::string accNum, double initialBalance = 0.0) 
        : accountNumber(accNum), balance(initialBalance) {
        if (initialBalance > 0) {
            recordTransaction("Deposit", initialBalance, "Initial Balance Setup");
        }
    }

    std::string getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    void recordTransaction(const std::string& type, double amount, const std::string& details = "") {
        transactionHistory.emplace_back(type, amount, details);
    }

    bool deposit(double amount) {
        if (amount <= 0) {
            std::cout << "Error: Deposit amount must be positive.\n";
            return false;
        }
        balance += amount;
        recordTransaction("Deposit", amount, "Self Deposit");
        return true;
    }

    bool withdraw(double amount, const std::string& customDetail = "Cash Withdrawal") {
        if (amount <= 0) {
            std::cout << "Error: Withdrawal amount must be positive.\n";
            return false;
        }
        if (amount > balance) {
            std::cout << "Error: Insufficient funds.\n";
            return false;
        }
        balance -= amount;
        recordTransaction("Withdrawal", amount, customDetail);
        return true;
    }

    void displayHistory() const {
        std::cout << "\n--- Transaction History for Account: " << accountNumber << " ---\n";
        if (transactionHistory.empty()) {
            std::cout << "No transactions recorded yet.\n";
            return;
        }
        std::cout << std::left << std::setw(15) << "Type" << std::setw(12) << "Amount" << "Details\n";
        std::cout << std::string(50, '-') << std::endl;
        for (const auto& tx : transactionHistory) {
            tx.displayTransaction();
        }
    }
};

// ==========================================
// 3. CUSTOMER CLASS
// ==========================================
class Customer {
private:
    std::string name;
    std::string customerId;
    std::vector<std::shared_ptr<Account>> accounts;

public:
    Customer(std::string n, std::string id) : name(n), customerId(id) {}

    std::string getName() const { return name; }
    std::string getCustomerId() const { return customerId; }
    std::vector<std::shared_ptr<Account>> getAccounts() const { return accounts; }

    void addAccount(const std::shared_ptr<Account>& account) {
        accounts.push_back(account);
    }

    std::shared_ptr<Account> findAccount(const std::string& accNum) {
        for (auto& acc : accounts) {
            if (acc->getAccountNumber() == accNum) {
                return acc;
            }
        }
        return nullptr;
    }

    void displayCustomerProfile() const {
        std::cout << "\n===============================\n";
        std::cout << "Customer ID: " << customerId << "\n";
        std::cout << "Name: " << name << "\n";
        std::cout << "Associated Accounts:\n";
        if (accounts.empty()) {
            std::cout << "  No active accounts.\n";
        } else {
            for (const auto& acc : accounts) {
                std::cout << "  - Account Number: " << acc->getAccountNumber() 
                          << " | Current Balance: $" << std::fixed << std::setprecision(2) << acc->getBalance() << "\n";
            }
        }
        std::cout << "===============================\n";
    }
};

// ==========================================
// 4. BANK SYSTEM MANAGEMENT CLASS
// ==========================================
class BankSystem {
private:
    std::vector<Customer> customers;
    std::vector<std::shared_ptr<Account>> allAccounts; // For global lookup across customers

    std::shared_ptr<Account> findGlobalAccount(const std::string& accNum) {
        for (auto& acc : allAccounts) {
            if (acc->getAccountNumber() == accNum) return acc;
        }
        return nullptr;
    }

public:
    void createCustomer(const std::string& name, const std::string& id) {
        for (const auto& c : customers) {
            if (c.getCustomerId() == id) {
                std::cout << "Error: Customer with ID " << id << " already exists.\n";
                return;
            }
        }
        customers.emplace_back(name, id);
        std::cout << "Customer '" << name << "' registered successfully.\n";
    }

    void openAccountForCustomer(const std::string& customerId, const std::string& accNum, double initialDeposit) {
        auto it = std::find_if(customers.begin(), customers.end(), [&](const Customer& c) {
            return c.getCustomerId() == customerId;
        });

        if (it == customers.end()) {
            std::cout << "Error: Customer ID not found.\n";
            return;
        }

        if (findGlobalAccount(accNum) != nullptr) {
            std::cout << "Error: Account number " << accNum << " already exists globally.\n";
            return;
        }

        auto newAccount = std::make_shared<Account>(accNum, initialDeposit);
        it->addAccount(newAccount);
        allAccounts.push_back(newAccount);
        std::cout << "Account " << accNum << " successfully opened for " << it->getName() << ".\n";
    }

    Customer* findCustomer(const std::string& id) {
        for (auto& c : customers) {
            if (c.getCustomerId() == id) return &c;
        }
        return nullptr;
    }

    void performTransfer(const std::string& fromAccNum, const std::string& toAccNum, double amount) {
        auto sourceAcc = findGlobalAccount(fromAccNum);
        auto destAcc = findGlobalAccount(toAccNum);

        if (!sourceAcc) {
            std::cout << "Transfer Failed: Source account not found.\n";
            return;
        }
        if (!destAcc) {
            std::cout << "Transfer Failed: Destination account not found.\n";
            return;
        }

        if (sourceAcc->withdraw(amount, "Transfer to " + toAccNum)) {
            destAcc->deposit(amount);
            // Overriding standard deposit string to reflect the transfer receipt
            destAcc->recordTransaction("Transfer", amount, "Received from " + fromAccNum);
            std::cout << "Successfully transferred $" << amount << " from " << fromAccNum << " to " << toAccNum << ".\n";
        }
    }
};

// ==========================================
// 5. INTERACTIVE CLI MENU
// ==========================================
int main() {
    BankSystem bank;
    int choice;

    // Seeding some initial demo data so the system isn't blank on launch
    bank.createCustomer("Alice Smith", "C001");
    bank.createCustomer("Bob Jones", "C002");
    bank.openAccountForCustomer("C001", "ACC101", 500.00);
    bank.openAccountForCustomer("C002", "ACC202", 150.00);

    do {
        std::cout << "\n========== BANKING SYSTEM MENU ==========\n";
        std::cout << "1. Add New Customer\n";
        std::cout << "2. Open New Account\n";
        std::cout << "3. Deposit Funds\n";
        std::cout << "4. Withdraw Funds\n";
        std::cout << "5. Transfer Funds\n";
        std::cout << "6. View Customer Profile & Balances\n";
        std::cout << "7. View Account Transaction History\n";
        std::cout << "8. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string name, id;
                std::cout << "Enter Customer Name: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Enter Unique Customer ID: ";
                std::cin >> id;
                bank.createCustomer(name, id);
                break;
            }
            case 2: {
                std::string id, accNum;
                double initialBalance;
                std::cout << "Enter Customer ID: ";
                std::cin >> id;
                std::cout << "Enter Desired Account Number: ";
                std::cin >> accNum;
                std::cout << "Enter Initial Deposit Amount: ";
                std::cin >> initialBalance;
                bank.openAccountForCustomer(id, accNum, initialBalance);
                break;
            }
            case 3: {
                std::string id, accNum;
                double amount;
                std::cout << "Enter Customer ID: ";
                std::cin >> id;
                Customer* cust = bank.findCustomer(id);
                if (cust) {
                    std::cout << "Enter Account Number: ";
                    std::cin >> accNum;
                    auto acc = cust->findAccount(accNum);
                    if (acc) {
                        std::cout << "Enter Deposit Amount: ";
                        std::cin >> amount;
                        if(acc->deposit(amount)) std::cout << "Deposit Successful!\n";
                    } else std::cout << "Account not found under this customer.\n";
                } else std::cout << "Customer not found.\n";
                break;
            }
            case 4: {
                std::string id, accNum;
                double amount;
                std::cout << "Enter Customer ID: ";
                std::cin >> id;
                Customer* cust = bank.findCustomer(id);
                if (cust) {
                    std::cout << "Enter Account Number: ";
                    std::cin >> accNum;
                    auto acc = cust->findAccount(accNum);
                    if (acc) {
                        std::cout << "Enter Withdrawal Amount: ";
                        std::cin >> amount;
                        if(acc->withdraw(amount)) std::cout << "Withdrawal Successful!\n";
                    } else std::cout << "Account not found under this customer.\n";
                } else std::cout << "Customer not found.\n";
                break;
            }
            case 5: {
                std::string fromAcc, toAcc;
                double amount;
                std::cout << "Enter Sender Account Number: ";
                std::cin >> fromAcc;
                std::cout << "Enter Recipient Account Number: ";
                std::cin >> toAcc;
                std::cout << "Enter Transfer Amount: ";
                std::cin >> amount;
                bank.performTransfer(fromAcc, toAcc, amount);
                break;
            }
            case 6: {
                std::string id;
                std::cout << "Enter Customer ID: ";
                std::cin >> id;
                Customer* cust = bank.findCustomer(id);
                if (cust) cust->displayCustomerProfile();
                else std::cout << "Customer not found.\n";
                break;
            }
            case 7: {
                std::string id, accNum;
                std::cout << "Enter Customer ID: ";
                std::cin >> id;
                Customer* cust = bank.findCustomer(id);
                if (cust) {
                    std::cout << "Enter Account Number: ";
                    std::cin >> accNum;
                    auto acc = cust->findAccount(accNum);
                    if (acc) acc->displayHistory();
                    else std::cout << "Account not found.\n";
                } else std::cout << "Customer not found.\n";
                break;
            }
            case 8:
                std::cout << "Exiting system. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid selection. Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}
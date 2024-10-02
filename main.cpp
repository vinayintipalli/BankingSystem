#include <iostream>
#include <thread>
#include <vector>

class BankingSystem {
public:
    explicit BankingSystem(int initial_balance) : balance(initial_balance) {}

    void deposit(int amount) {
        balance += amount;
    }

    void withdraw(int amount) {
        if (balance >= amount) {
            balance -= amount;
        } else {
            std::cout << "Insufficient balance for withdrawal!\n";
        }
    }

    void printBalance() const {
        std::cout << "Final Balance: " << balance << std::endl;
    }

private:
    int balance;
};

int main() {
    BankingSystem bankingSystem(0);
    std::vector<std::thread> threads;

    while (threads.size() <= 10) {
        threads.emplace_back(&BankingSystem::deposit, &bankingSystem, 10);
        threads.emplace_back(&BankingSystem::withdraw, &bankingSystem, 10);
    }

    for (auto& threadItem : threads) {
        threadItem.join();
    }

    bankingSystem.printBalance();

    return 0;
}

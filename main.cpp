#include <iostream>
#include <thread>
#include <vector>
#include <shared_mutex>
#include <mutex>

class BankingSystem {
public:
    explicit BankingSystem(int initial_balance) : balance(initial_balance) {}

    void deposit(int amount) {
        std::unique_lock<std::shared_mutex> ul(balance_mutex);
        balance += amount;
    }

    void withdraw(int amount) {
        std::unique_lock<std::shared_mutex> ul(balance_mutex);
        if (balance >= amount) {
            balance -= amount;
        } else {
            std::cout << "Insufficient balance for withdrawal!\n";
        }
    }

    void printBalance() const {
        std::shared_lock<std::shared_mutex> sm(balance_mutex);
        std::cout << "Final Balance: " << balance << std::endl;
    }

private:
    int balance;
    mutable std::shared_mutex balance_mutex;
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

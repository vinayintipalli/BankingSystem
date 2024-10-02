#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

class BankingSystem {
public:
    explicit BankingSystem(int initial_balance) : balance(initial_balance) {}

    void deposit(int amount) {
        int currentBalance = balance.load();
        int updatedBalance = currentBalance + amount;
        while(not balance.compare_exchange_weak(currentBalance, updatedBalance)) {
            updatedBalance = currentBalance + amount;    
        }
        // compare_exchange_weak attempts to atomically set balance to updatedBalance, 
        // but only if balance is still equal to currentBalance. 
        // If balance was changed by another thread in the meantime, the operation will fail, 
        // and currentBalance will be updated with the current value of balance.
    }

    void withdraw(int amount) {
        int currentBalance = balance.load();
        int updatedBalance = currentBalance - amount;
        if (currentBalance < amount) {
            std::cout << "Insufficient balance for withdrawal!";
            return;
        }
        while(not balance.compare_exchange_weak(currentBalance, updatedBalance)) {
            if (currentBalance < amount) {
                std::cout << "Insufficient balance for withdrawal!";
                return;
            }
            updatedBalance = currentBalance - amount;    
        }
    }

    void printBalance() const {
        std::cout << "Final Balance: " << balance.load() << std::endl;
    }

private:
    std::atomic<int> balance;
};

int main() {
    BankingSystem bankingSystem(0);
    std::vector<std::thread> threads;

    while (threads.size() <= 20) {
        threads.emplace_back(&BankingSystem::deposit, &bankingSystem, 10);
        threads.emplace_back(&BankingSystem::withdraw, &bankingSystem, 10);
    }

    for (auto& threadItem : threads) {
        threadItem.join();
    }

    bankingSystem.printBalance();
    return 0;
}

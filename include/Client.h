#pragma once

#include <string>
#include <vector>

class Account;

class Client {
public:
    Client(int id, const std::string& fullName);

    int getId() const;
    const std::string& getFullName() const;

    // Агрегация: клиент хранит ссылки на уже существующие счета.
    void addAccount(Account* account);
    const std::vector<Account*>& getAccounts() const;

    double totalBalance() const;

private:
    int id_;
    std::string fullName_;
    std::vector<Account*> accounts_;
};

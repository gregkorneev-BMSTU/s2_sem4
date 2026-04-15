#include "Client.h"

#include "Account.h"

Client::Client(int id, const std::string& fullName) : id_(id), fullName_(fullName) {}

int Client::getId() const {
    return id_;
}

const std::string& Client::getFullName() const {
    return fullName_;
}

void Client::addAccount(Account* account) {
    if (account != nullptr) {
        accounts_.push_back(account);
    }
}

const std::vector<Account*>& Client::getAccounts() const {
    return accounts_;
}

double Client::totalBalance() const {
    double sum = 0;
    for (std::size_t i = 0; i < accounts_.size(); i++) {
        sum += accounts_[i]->getBalance();
    }
    return sum;
}

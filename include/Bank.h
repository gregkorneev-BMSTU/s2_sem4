#pragma once

#include <string>
#include <vector>

#include "CommissionCalculator.h"
#include "Department.h"

class Account;
class Client;

class Bank {
public:
    Bank(const std::string& name);

    const std::string& getName() const;

    // Композиция: отделения принадлежат банку.
    void addDepartment(const std::string& name, const std::string& address);
    const std::vector<Department>& getDepartments() const;

    void registerClient(Client& client);

    // Делегирование: часть работы (комиссия) отдается CommissionCalculator.
    bool transfer(Account& from, Account& to, double amount);

private:
    std::string name_;
    CommissionCalculator commissionCalculator_;
    std::vector<Department> departments_;
    std::vector<Client*> clients_;
};

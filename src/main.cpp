#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Account.h"
#include "Bank.h"
#include "Card.h"
#include "Client.h"
#include "CreditAccount.h"
#include "DepositAccount.h"
#include "SavingsAccount.h"

namespace {

const std::string RESET  = "\033[0m";
const std::string BOLD   = "\033[1m";
const std::string CYAN   = "\033[36m";
const std::string GREEN  = "\033[32m";
const std::string RED    = "\033[31m";
const std::string YELLOW = "\033[33m";

std::string formatMoney(double value) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << value << " RUB";
    return out.str();
}

void printLine(char ch = '=', int width = 80) {
    for (int i = 0; i < width; i++) {
        std::cout << ch;
    }
    std::cout << '\n';
}

void printCentered(const std::string& text, int width = 80) {
    int padding = static_cast<int>((width - static_cast<int>(text.size())) / 2);
    if (padding < 0) {
        padding = 0;
    }
    std::cout << std::string(padding, ' ') << text << '\n';
}

void printSection(const std::string& title) {
    std::cout << '\n' << CYAN << BOLD;
    printLine('=');
    printCentered(title);
    printLine('=');
    std::cout << RESET;
}

void printSubsection(const std::string& title) {
    std::cout << '\n' << YELLOW << BOLD << title << RESET << '\n';
    printLine('-');
}

std::string transactionTypeToText(TransactionType type) {
    if (type == TransactionType::Deposit) {
        return "Пополнение";
    }
    if (type == TransactionType::Withdrawal) {
        return "Снятие";
    }
    if (type == TransactionType::TransferIn) {
        return "Входящий перевод";
    }
    if (type == TransactionType::TransferOut) {
        return "Исходящий перевод";
    }
    if (type == TransactionType::Fee) {
        return "Комиссия";
    }
    return "Операция";
}

void printInfo(const std::string& text) {
    std::cout << YELLOW << "[INFO]" << RESET << " " << text << '\n';
}

void printAccountCard(const Account& account) {
    printLine('-');
    std::cout << std::left
              << std::setw(16) << "Тип:"         << account.getDisplayType() << '\n'
              << std::setw(16) << "Номер:"       << account.getNumber() << '\n'
              << std::setw(16) << "Владелец:"    << account.getOwnerName() << '\n'
              << std::setw(16) << "Баланс:"      << formatMoney(account.getBalance()) << '\n'
              << std::setw(16) << "Особенность:" << account.getSpecialInfo() << '\n';
    printLine('-');
}

void showAccountPolymorphically(const Account& account) {
    std::cout << "Работа через базовый тип Account:\n";
    printAccountCard(account);
}

void useDerivedFeature(Account& account) {
    std::cout << "Использование dynamic_cast: ";

    if (SavingsAccount* savings = dynamic_cast<SavingsAccount*>(&account)) {
        savings->addMonthlyInterest();
        std::cout << "для SavingsAccount вызван addMonthlyInterest().\n";
        return;
    }

    if (CreditAccount* credit = dynamic_cast<CreditAccount*>(&account)) {
        std::cout << "для CreditAccount получен лимит через getCreditLimit(): "
                  << formatMoney(credit->getCreditLimit()) << ".\n";
        return;
    }

    if (DepositAccount* deposit = dynamic_cast<DepositAccount*>(&account)) {
        deposit->unlock();
        std::cout << "для DepositAccount вызван unlock().\n";
        return;
    }

    std::cout << "тип не распознан.\n";
}

void printDepartments(const Bank& bank) {
    const std::vector<Department>& deps = bank.getDepartments();

    std::cout << std::left
              << std::setw(5)  << "№"
              << std::setw(18) << "Отделение"
              << std::setw(50) << "Адрес"
              << '\n';
    printLine('-');

    for (std::size_t i = 0; i < deps.size(); i++) {
        std::cout << std::left
                  << std::setw(5)  << (i + 1)
                  << std::setw(18) << deps[i].getName()
                  << std::setw(50) << deps[i].getAddress()
                  << '\n';
    }
}

void printOperationResult(const std::string& icon,
                          const std::string& text,
                          bool success) {
    std::string fullText = icon + " " + text;
    std::cout << std::left << std::setw(62) << fullText << ' ';

    if (success) {
        std::cout << GREEN << "[OK]" << RESET;
    } else {
        std::cout << RED << "[ERROR]" << RESET;
    }

    std::cout << '\n';
}

void printClientSummary(const Client& client) {
    printLine('-');
    std::cout << std::left
              << std::setw(14) << "Клиент:" << client.getFullName() << '\n'
              << std::setw(14) << "ID:"     << client.getId() << '\n'
              << std::setw(14) << "Счетов:" << client.getAccounts().size() << '\n'
              << std::setw(14) << "Итого:"  << formatMoney(client.totalBalance()) << '\n';
    printLine('-');
}

void printHistoryTable(const Account& account) {
    printSubsection("История счета " + account.getNumber());

    const std::vector<Transaction>& history = account.getHistory();

    if (history.empty()) {
        std::cout << "История операций пуста.\n";
        return;
    }

    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(25) << "Тип"
              << std::setw(18) << "Сумма"
              << std::setw(33) << "Комментарий"
              << '\n';
    printLine('-');

    for (std::size_t i = 0; i < history.size(); i++) {
        const Transaction& tx = history[i];

        std::cout << std::left
                  << std::setw(6)  << tx.getId()
                  << std::setw(25) << transactionTypeToText(tx.getType())
                  << std::setw(18) << formatMoney(tx.getAmount())
                  << std::setw(33) << tx.getComment()
                  << '\n';
    }
}

} // namespace

int main() {
    printSection("БАНКОВСКАЯ СИСТЕМА (ВАРИАНТ 15)");

    Bank bank("BMSTU Bank");
    bank.addDepartment("Центральное", "Москва, ул. Бауманская, 5");
    bank.addDepartment("Северное", "Москва, ул. Лобненская, 12");

    Client alice(1, "Алиса Иванова");
    Client bob(2, "Боб Петров");

    bank.registerClient(alice);
    bank.registerClient(bob);

    SavingsAccount aliceSavings("ACC-1001", alice.getFullName(), 50000, 6);
    CreditAccount bobCredit("ACC-2001", bob.getFullName(), 15000, 30000);
    DepositAccount aliceDeposit("ACC-3001", alice.getFullName(), 120000, 12);

    alice.addAccount(&aliceSavings);
    alice.addAccount(&aliceDeposit);
    bob.addAccount(&bobCredit);

    Card aliceCard("2200 1000 0000 0001", aliceSavings);

    printSection("1. ОТДЕЛЕНИЯ БАНКА");
    printDepartments(bank);

    printSection("2. НАЧАЛЬНОЕ СОСТОЯНИЕ СЧЕТОВ");
    printAccountCard(aliceSavings);
    printAccountCard(bobCredit);
    printAccountCard(aliceDeposit);

    printSection("3. ВЫПОЛНЕНИЕ ОПЕРАЦИЙ");
    printOperationResult("💳", "Оплата картой 1500 RUB (магазин: Книжный)",
                         aliceCard.pay(1500, "Книжный"));

    printOperationResult("💰", "Пополнение Savings на 3000 RUB",
                         aliceSavings.deposit(3000, "Внесение наличных"));

    printOperationResult("💸", "Снятие с Credit 20000 RUB",
                         bobCredit.withdraw(20000, "Покупка техники"));

    printOperationResult("⛔", "Попытка снять с Deposit 1000 RUB до разблокировки",
                         aliceDeposit.withdraw(1000, "Попытка досрочного снятия"));

    aliceDeposit.unlock();

    printOperationResult("🔓", "Снятие с Deposit 1000 RUB после разблокировки",
                         aliceDeposit.withdraw(1000, "Снятие после разблокировки"));

    printOperationResult("🔄", "Перевод 5000 RUB со Savings на Credit",
                         bank.transfer(aliceSavings, bobCredit, 5000));

    aliceSavings.addMonthlyInterest();
    printInfo("На сберегательный счет начислены месячные проценты.");

    printSection("4. ИТОГОВОЕ СОСТОЯНИЕ СЧЕТОВ");
    printAccountCard(aliceSavings);
    printAccountCard(bobCredit);
    printAccountCard(aliceDeposit);

    printSection("5. СВОДКА ПО КЛИЕНТАМ");
    printClientSummary(alice);
    printClientSummary(bob);

    printSection("6. ДЕМОНСТРАЦИЯ ПОЛИМОРФИЗМА");
    std::vector<std::unique_ptr<Account>> trainingAccounts;
    trainingAccounts.push_back(
        std::make_unique<SavingsAccount>("ACC-4001", "Учебный клиент", 20000, 4.5));
    trainingAccounts.push_back(
        std::make_unique<CreditAccount>("ACC-4002", "Учебный клиент", 10000, 25000));
    trainingAccounts.push_back(
        std::make_unique<DepositAccount>("ACC-4003", "Учебный клиент", 50000, 9));

    for (std::unique_ptr<Account>& account : trainingAccounts) {
        showAccountPolymorphically(*account);
        useDerivedFeature(*account);
        std::cout << "Состояние после специфичного действия:\n";
        printAccountCard(*account);
        printLine('.');
    }

    printSection("7. ИСТОРИЯ ОПЕРАЦИЙ");
    printHistoryTable(aliceSavings);
    printHistoryTable(bobCredit);
    printHistoryTable(aliceDeposit);

    printSection("8. ИТОГ ПО БАНКУ");
    double totalBankBalance = alice.totalBalance() + bob.totalBalance();
    std::cout << BOLD << "Общий баланс по всем клиентам: " << RESET
              << formatMoney(totalBankBalance) << '\n';

    printSection("РАБОТА ПРОГРАММЫ ЗАВЕРШЕНА");
    return 0;
}

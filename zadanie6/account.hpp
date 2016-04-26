#ifndef __gsb__account__
#define __gsb__account__

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "currency.hpp"
#include "citizen.hpp"

/* AccountInfo struct */
struct AccountInfo {
    double monthlyCharge;
    double transferCharge;
    double interestRate;
    AccountInfo() : monthlyCharge(0), transferCharge(0), interestRate(0) {}
};


class Bank; // Cyclic dependency;
            // Should be resolved by DI ...


/* Account interface */
class Account {
public:
    // Member types
    typedef std::size_t id_t;
    typedef std::pair<double, Currency> amount_t;
    enum Type : std::size_t { SAVING, CHECKING, CURRENCY, TYPES };

    // Destructor
    virtual ~Account() {};

    // Returns an ID of the account
    virtual Account::id_t id() const = 0;

    // Returns a type of the account
    virtual Account::Type type() const = 0;

    // Returns a currency stored on the account
    virtual Currency currency() const = 0;

    // Transfer
    virtual void transfer(double, Account::id_t, const std::string&) = 0;
    virtual void transfer(const amount_t&, Account::id_t, const std::string&) = 0;
    virtual void transfer(double, const std::string&, const std::string&) = 0;
    virtual void transfer(const amount_t&, const std::string&, const std::string&) = 0;

    // Deposit
    virtual void deposit(double amount) = 0;
    virtual void deposit(const amount_t& amount) = 0;

    // Withrawal
    virtual void withdraw(double amount) = 0;
    virtual void withdraw(const amount_t& amount) = 0;

    // Returns a current balance of the account
    virtual double balance() const = 0;

    // Returns all financial operations performed
    virtual const std::string history() const = 0;
};

/* Output stream insertion operator */
inline std::ostream& operator<<(std::ostream& os, const Account& account) {
    return os << "ACCOUNT: " << std::to_string(account.id()) << std::endl
        << "balance: " << account.balance() << std::endl
        << "history: " << std::endl
        << account.history();
}


/* SavingAccount class */
class SavingAccount : public Account {
public:
    // Constructors
    SavingAccount(Account::id_t id, Citizen::id_t owner_id);
    SavingAccount(SavingAccount&& account);

    // Destructor
    ~SavingAccount();

    // Returns an ID of the account
    Account::id_t id() const;

    // Returns a type of the account
    virtual Account::Type type() const;

    // Returns a currency stored on the account
    virtual Currency currency() const;

    // Transfer
    void transfer(double amount, Account::id_t dst_id, const std::string& title = "");
    void transfer(double amount, const std::string& dst_id, const std::string& title = "");
    void transfer(const amount_t& amount, Account::id_t dst_id, const std::string& title = "");
    void transfer(const amount_t& amount, const std::string& dst_id, const std::string& title = "");

    // Deposit
    virtual void deposit(double amount);
    virtual void deposit(const amount_t& amount);

    // Withrawal
    virtual void withdraw(double amount);
    virtual void withdraw(const amount_t& amount);

    // Returns a current balance of the account
    double balance() const;

    // Returns all financial operations performed
    const std::string history() const;

private:
    // An ID of the account
    const Account::id_t _id;

    // An ID of the owner
    const Citizen::id_t _owner_id;

protected:
    // Current balance of the account
    double _balance;
};


/* CheckingAccount class */
class CheckingAccount : public SavingAccount {
public:
    // Constructors
    CheckingAccount(Account::id_t id, Citizen::id_t owner_id);
    CheckingAccount(CheckingAccount&& account);

    // Returns a type of the account
    virtual Account::Type type() const;

    // Deposit
    void deposit(double amount);
    virtual void deposit(const amount_t& amount);

    // Withrawal
    void withdraw(double amount);
    virtual void withdraw(const amount_t& amount);
};


/* CurrencyAccount class */
class CurrencyAccount : public CheckingAccount {
public:
    // Constructors
    CurrencyAccount(Account::id_t id, Citizen::id_t owner_id, Currency currency);
    CurrencyAccount(CurrencyAccount&& account);

    // Returns a type of the account
    Account::Type type() const;

    // Returns a currency stored on the account
    Currency currency() const;

    // Deposit
    void deposit(const amount_t& amount);

    // Withrawal
    void withdraw(const amount_t& amount);

protected:
    // A currency stored on the account
    const Currency _currency;
};


#endif /* defined(__gsb__account__) */

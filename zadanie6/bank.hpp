#ifndef __gsb__bank__
#define __gsb__bank__

#include <cstddef>
#include <string>
#include <map>
#include <set>
#include "observe.hpp"
#include "citizen.hpp"
#include "currency.hpp"
#include "account.hpp"

/* Bank class */
class Bank : public Observer {
public:
    // Constructor 
    Bank(const std::string& name, const AccountInfo* settings);
    
    // Destructor
    ~Bank();
    
    // Returns current buying rate of a given currency
    double buyingRate(Currency currency) const;
    
    // Returns current selling rate of a given currency
    double sellingRate(Currency currency) const;
    
    // Returns an exchange table
    ExchangeTable& exchangeTable();

    // Creates a new saving account
    Account& openSavingAccount(const Citizen& owner);
    
    // Creates a new checking account
    Account& openCheckingAccount(const Citizen& owner);
    
    // Creates a new currency account
    Account& openCurrencyAccount(const Citizen& owner, Currency currency);
    
    // Transfer
    void transfer(Account::id_t src_id, Account::id_t dst_id, double amount, 
	              Currency currency, const std::string& title= "") const;

    // Withdrawal
    void withdrawal(Account::id_t id, double amount) const;

    // Deposit
    void deposit(Account::id_t id, double amount) const;
    
    // Finds an account
    static Account& getAccount(Account::id_t id);
    
    // An observed object has changed its state
    void notify() const;

private:
    // A name of the bank
    const std::string _name;

    // Current params of an offered accounts
    AccountInfo _settings[Account::Type::TYPES];

    // An exchange table
    ExchangeTable _exchange_table;

    // Accounts provided by the bank
    std::set<Account*> _accounts;

    // Map of all accounts in the galaxy
    static std::map<Account::id_t, Account*> all_accounts;
};


/* Gkb class */
class Gkb {
public:
    // Member type
    class BankBuilder;

    // Provides an application for creating a new bank
    BankBuilder& bankApplication() const;
};

/* Returns the instance of Gkb class */
Gkb& gkb();


/* Gkb::BankBuilder class */
class Gkb::BankBuilder {
public:
    // Constructor
    BankBuilder();
    
    // Sets a name of the bank
    BankBuilder& name(const std::string& bank_name);

    // Saving account settings
    BankBuilder& savingAccount();
    
    // Checking account settings
    BankBuilder& checkingAccount();

    // Currency account settings
    BankBuilder& currencyAccount();
    
    // Sets an account maintenance fee
    BankBuilder& monthlyCharge(double value);
    
    // Sets a transfer fee
    BankBuilder& transferCharge(double value);
    
    // Sets a interest rate
    BankBuilder& interestRate(double value);
    
    // Creates a new bank
    Bank& createBank();

private:
    // A name for a created bank
    std::string _bank_name;

    // Currently setting
    Account::Type _account_type;

    // Params of offered accounts
    AccountInfo _settings[Account::Type::TYPES];
};


#include <exception>

/* Hierarchy of bank exceptions */
class BankException : public std::exception {};
class InvalidValueException : public BankException {};
class InvalidOperationException : public BankException {};
class IncorrectCurrencyException : public BankException {};
class AccountNotFoundException : public BankException {};
class NonsufficientFundsException : public BankException {};


#endif /* defined(__gsb__bank__) */

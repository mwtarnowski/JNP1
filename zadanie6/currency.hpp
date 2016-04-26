#ifndef __gsb__currency__
#define __gsb__currency__

#include <cstddef>
#include <iosfwd>


/* Currency enumeration */
enum Currency : std::size_t { ENC, BIC, DIL, LIT, CURRENCIES };

/* Output stream insertion operator for currency */
std::ostream& operator<<(std::ostream& os, Currency currency);


/* ExchangeInfo class */
class ExchangeInfo {
public:
    // Constructor
    ExchangeInfo();

    // Returns current buying rate
    double buyingRate() const;

    // Returns current selling rate
    double sellingRate() const;

    // Changes current buying rate
    void buyingRate(double value);

    // Changes current selling rate
    void sellingRate(double value);

private:
    // A buying rate
    double _buying_rate;

    // A selling rate
    double _selling_rate;
};


/* ExchangeTable class */
class ExchangeTable {
public:
    // Constructor
    ExchangeTable();

    // Returns an exchange table for a given currency
    ExchangeTable& exchangeRate(Currency currency);

    // Returns current buying rate
    double buyingRate(Currency currency) const;

    // Returns current selling rate
    double sellingRate(Currency currency) const;

    // Changes current buying rate
    ExchangeTable& buyingRate(double value);

    // Changes current selling rate
    ExchangeTable& sellingRate(double value);

private:
    // A currently displayed currency
    Currency _currency;

    // Rates of all currencies
    ExchangeInfo _rates[Currency::CURRENCIES];
};


#endif /* defined(__gsb__currency__) */

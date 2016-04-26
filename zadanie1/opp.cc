#include <cstddef>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <algorithm>
//#include <regex>
#include <boost/regex.hpp>


/* TYPES DEFINITIONS */
typedef std::string name_t;
typedef double amount_t;
typedef std::string symbol_t;
typedef std::tuple<name_t, amount_t, symbol_t, amount_t> donation_t;


// Tricky construction: struct + unnamed plain enum type instead of enum 
// class to avoid an explicit casting on underlaying type: std::size_t.
struct Donation {
    enum : std::size_t { DONOR = 0, AMOUNT = 1, SYMBOL = 2, VALUE = 3 };
};


/* ISO 4217 CODES */
const std::vector<symbol_t> ISO =  {"AFN","EUR","ALL","DZD","USD","AOA","XCD","XCD","ARS",
"AMD","AWG","AUD","AZN","BSD","BHD","BDT","BBD","BYR","BZD","XOF","BMD","BTN","INR","BOB",
"BOV","BAM","BWP","NOK","BRL","BND","BGN","XOF","BIF","CVE","KHR","XAF","CAD","KYD","XAF",
"XAF","CLF","CLP","CNY","AUD","COP","COU","KMF","CDF","XAF","NZD","CRC","XOF","HRK","CUC",
"CUP","ANG","CZK","DKK","DJF","DOP","EGP","SVC","XAF","ERN","ETB","FKP","DKK","FJD","XPF",
"XAF","GMD","GEL","GHS","GIP","DKK","XCD","GTQ","GBP","GNF","XOF","GYD","HTG","AUD","HNL",
"HKD","HUF","ISK","INR","IDR","XDR","IRR","IQD","GBP","ILS","JMD","JPY","GBP","JOD","KZT",
"KES","AUD","KPW","KRW","KWD","KGS","LAK","LBP","LSL","ZAR","LRD","LYD","CHF","MOP","MKD",
"MGA","MWK","MYR","MVR","XOF","MRO","MUR","XUA","MXN","MXV","MDL","MNT","XCD","MAD","MZN",
"MMK","NAD","ZAR","AUD","NPR","XPF","NZD","NIO","XOF","NGN","NZD","AUD","NOK","OMR","PKR",
"PAB","PGK","PYG","PEN","PHP","NZD","PLN","QAR","RON","RUB","RWF","SHP","XCD","XCD","WST",
"STD","SAR","XOF","RSD","SCR","SLL","SGD","ANG","XSU","SBD","SOS","ZAR","SSP","LKR","SDG",
"SRD","NOK","SZL","SEK","CHE","CHF","CHW","SYP","TJS","TZS","THB","XOF","NZD","TOP","TTD",
"TND","TRY","TMT","AUD","UGX","UAH","AED","GBP","USN","UYI","UYU","UZS","VUV","VEF","VND",
"XPF","MAD","YER","ZMW","ZWL","XBA","XBB","XBC","XBD","XTS","XXX","XAU","XPD","XPT","XAG"};


/* CONSTANT VALUES */
const amount_t MAXAMOUNT = 524288;
const amount_t MAXQUERY  = 274877906944;
const amount_t EPSILON   = 0.001;
const std::streamsize PRECISION = 3;


/* REGULAR EXPRESSIONS */
const std::string NAME_PATTERN = "(\\S+(\\s+\\S+)*)";
const std::string SYMB_PATTERN = "([A-Z]{3})";
const std::string NUMB_PATTERN = "[0]*([\\d]{1,12}(,[\\d]{1,3})?)";
// Explanation:                                ^^           ^ 
// One can assume that all valid values occurring on input are 10-based numbers 
// with at most 3 digits after decimal point from the range (0, 274877906944).
// See: MAXQUERY


/* GLOBAL CONTAINERS */
std::map<symbol_t, amount_t> exchange;       // Exchange table
std::vector<donation_t> donations;           // List of all donations
std::pair<amount_t, amount_t> bounds;	     // Current query info


// Banker's Rounding
// Rounds the amount to the nearest integer. Half-way values are rounded 
// toward the nearest even number.
amount_t roundAmount(const amount_t& value) {
    int remainder = static_cast<int>(std::round(10 * value) - 10 * std::floor(value));
    if (remainder < 5)
        return std::floor(value);
    if (remainder > 5)
        return std::ceil(value);
    bool is_even = static_cast<int>(std::fmod(value, 2)) == 0;
    return is_even ? std::floor(value) : std::ceil(value);
}


// Compares two donations based on approximate local value.
bool donationComparer(const donation_t& x, const donation_t& y) {
    return std::get<Donation::VALUE>(x) < std::get<Donation::VALUE>(y);
}


// Returns an amount extracted from the string
amount_t stringToValue(std::string str) {
    std::size_t comma_pos = str.find(",");
    if (comma_pos != std::string::npos)
        str.replace(comma_pos, 1, ".");
    return std::stod(str);
}


// Converts the amount to the string
std::string valueToString(const amount_t& val) {
    std::stringstream ss;
    ss.setf(std::ios::fixed, std::ios::floatfield);
    ss.precision(PRECISION);
    ss << val;
    std::string result = ss.str();
    std::size_t dot_pos = result.find(".");
    if (dot_pos != std::string::npos)
        result.replace(dot_pos, 1, ",");
    return result;
}



// If delivered string contains a valid exchange rate puts it into the 
// exchange table and returns true, otherwise returns false.
bool getCurrency(const std::string& line) {
    static const boost::regex RATING_PATTERN(
        "^\\s*" + SYMB_PATTERN + "\\s+" + NUMB_PATTERN + "\\s*$");
    enum : std::size_t { SYMBOL = 1, RATE = 2 };
    boost::smatch match;

    if(!boost::regex_match(line, match, RATING_PATTERN))
        return false;

    symbol_t symbol = match[SYMBOL].str();
    amount_t rating = stringToValue(match[RATE].str());

    if(rating < EPSILON || rating > MAXAMOUNT)
        return false;

    if(std::find(ISO.begin(), ISO.end(), symbol) == ISO.end())
        return false;

    if(exchange.count(symbol) != 0)
        return false;

    std::pair<symbol_t, amount_t> currency = std::make_pair(symbol, rating);
    exchange.insert(currency);
    return true;
}


// If delivered string contains a valid donation entry adds an information 
// on it to the list of all donations and returns true, otherwise returns false.
bool getDonation(const std::string& line) {
    static const boost::regex DONATION_PATTERN(
        "^\\s*" + NAME_PATTERN + "\\s+" + NUMB_PATTERN + "\\s+" + SYMB_PATTERN + "\\s*$");
    enum : std::size_t { NAME = 1, AMOUNT = 3, SYMBOL = 5 };
    boost::smatch match;

    if(!boost::regex_match(line, match, DONATION_PATTERN))
        return false;

    name_t donor_name = match[NAME].str();
    amount_t amount   = stringToValue(match[AMOUNT].str());
    symbol_t currency = match[SYMBOL].str();

    if(amount < EPSILON || amount > MAXAMOUNT)
        return false;

    if(exchange.count(currency) == 0)
        return false;

    amount_t value = amount * exchange[currency];
    value = roundAmount(1000.0 * value) / 1000.0;
    donation_t donation = std::make_tuple(donor_name, amount, currency, value);
    donations.push_back(donation);
    return true;
}


// If delivered string contains a valid query saves it into global variable 
// and returns true, otherwise returns false.
bool getQuery(const std::string& line) {
    static const boost::regex QUERY_PATTERN(
        "^\\s*" + NUMB_PATTERN + "\\s+" + NUMB_PATTERN + "\\s*$");
    enum : std::size_t { LBOUND = 1, RBOUND = 3 };
    boost::smatch match;

    if(!boost::regex_match(line, match, QUERY_PATTERN))
        return false;

    amount_t min = stringToValue(match[LBOUND].str());
    amount_t max = stringToValue(match[RBOUND].str());

    if (min > max)
        return false;

    if(max > MAXQUERY)
        return false;

    bounds.first  = min;
    bounds.second = max;
    return true;
}


// Prints the text information about the donation.
void printDonation(const donation_t& donation) {
    std::cout << "\"" << std::get<Donation::DONOR>(donation) << "\","
        << "\"" << valueToString(std::get<Donation::AMOUNT>(donation)) << "\","
        << std::get<Donation::SYMBOL>(donation) << std::endl;
}


// Puts an error report to the standard error stream.
void reportError(const std::size_t& number, const std::string& line) {
    std::cerr << "Error in line " << number << ":" << line << std::endl;
}



int main() {
    std::string line;
    std::size_t line_no;
    enum { CURRENCY, DONATION, QUERY } input_mode;


    // READING DATA
    line_no = 0;
    input_mode = CURRENCY;
    while (std::getline(std::cin, line)) {
        line_no++;
        if (input_mode == CURRENCY) 
        {
            if (getCurrency(line))         continue;
            else if (getDonation(line))    input_mode = DONATION;
            else if (getQuery(line))       input_mode = QUERY;
            else reportError(line_no, line);
        }
        else if (input_mode == DONATION) 
        {
            if (getDonation(line))         continue;
            else if (getQuery(line))       input_mode = QUERY;
            else reportError(line_no, line);
        }
        if (input_mode == QUERY) break;
    }


    // PROCESSING DATA
    std::stable_sort(donations.begin(), donations.end(), donationComparer);


    // READING & PROCESSING QUERIES
    donation_t bound_wrapper;
    if (input_mode == QUERY) do {
        if (line.empty()) continue;
        if (getQuery(line)) {

            std::get<Donation::VALUE>(bound_wrapper) = bounds.first;
            auto start = std::lower_bound(donations.begin(), donations.end(),
                bound_wrapper, donationComparer);

            std::get<Donation::VALUE>(bound_wrapper) = bounds.second;
            auto end = std::upper_bound(donations.begin(), donations.end(),
                bound_wrapper, donationComparer);

            std::for_each(start, end, printDonation);
        }
        else reportError(line_no, line);
        line_no++;
    } while (std::getline(std::cin, line));


    return 0;
}

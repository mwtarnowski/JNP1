#ifndef VERY_LONG_INT_HH
#define VERY_LONG_INT_HH

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

class VeryLongInt
{
private:
    typedef std::int64_t digit_t;

    static const int BASE;          // Numeral system radix
    static const int LOGB;          // Binary logarithm of the radix

    std::vector<digit_t> digits;    // Vector of digits

    /* Helper methods */
    void align();
    void lshift(std::size_t);
    void rshift(std::size_t);

public:
    /* Constructors */
    VeryLongInt();
    VeryLongInt(const VeryLongInt&);
    VeryLongInt(VeryLongInt&&);

    VeryLongInt(short);
    VeryLongInt(int);
    VeryLongInt(long);
    VeryLongInt(long long);
    VeryLongInt(unsigned short);
    VeryLongInt(unsigned int);
    VeryLongInt(unsigned long);
    VeryLongInt(unsigned long long);

    VeryLongInt(bool)          = delete;
    VeryLongInt(char)          = delete;
    VeryLongInt(signed char)   = delete;
    VeryLongInt(unsigned char) = delete;

    explicit VeryLongInt(const std::string&);
    explicit VeryLongInt(const char*);

    /* Destructor */
    ~VeryLongInt() = default;

    /* Basic assignment operator */
    VeryLongInt& operator =(const VeryLongInt&);
    VeryLongInt& operator =(VeryLongInt&&);

    /* Compound arithmetic assignment operators */
    VeryLongInt& operator +=(const VeryLongInt&);
    VeryLongInt& operator -=(const VeryLongInt&);
    VeryLongInt& operator *=(const VeryLongInt&);
    VeryLongInt& operator /=(const VeryLongInt&);
    VeryLongInt& operator %=(const VeryLongInt&);

    /* Compound bitshift assignment operators */
    VeryLongInt& operator >>=(unsigned int);
    VeryLongInt& operator <<=(unsigned int);

    /* Number of binary digits */
    std::size_t numberOfBinaryDigits() const;

    /* Conversion operator */
    explicit operator bool() const;

    /* Test validation */
    bool isValid() const;

//friend:
    /* Relational operators */
    friend bool operator ==(const VeryLongInt&, const VeryLongInt&);
    friend bool operator  <(const VeryLongInt&, const VeryLongInt&);

    /* Stream insertion operator */
    friend std::ostream& operator <<(std::ostream&, const VeryLongInt&);
};

/* Arithmetic operators */
const VeryLongInt operator +(const VeryLongInt&, const VeryLongInt&);
const VeryLongInt operator -(const VeryLongInt&, const VeryLongInt&);
const VeryLongInt operator *(const VeryLongInt&, const VeryLongInt&);
const VeryLongInt operator /(const VeryLongInt&, const VeryLongInt&);
const VeryLongInt operator %(const VeryLongInt&, const VeryLongInt&);

/* Bitwise shift operators */
const VeryLongInt operator >>(const VeryLongInt&, unsigned int);
const VeryLongInt operator <<(const VeryLongInt&, unsigned int);

/* Relational operators */
bool operator ==(const VeryLongInt&, const VeryLongInt&);
bool operator !=(const VeryLongInt&, const VeryLongInt&);
bool operator  <(const VeryLongInt&, const VeryLongInt&);
bool operator  >(const VeryLongInt&, const VeryLongInt&);
bool operator <=(const VeryLongInt&, const VeryLongInt&);
bool operator >=(const VeryLongInt&, const VeryLongInt&);

/* Stream insertion operator */
std::ostream& operator <<(std::ostream&, const VeryLongInt&);

/* Global NAN & ZERO */
const VeryLongInt& NaN();
const VeryLongInt& Zero();

#endif /* VERY_LONG_INT_HH */

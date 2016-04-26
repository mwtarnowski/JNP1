#include "very_long_int.hh"
#include <ostream>
#include <iomanip>
#include <cmath>
#include <cctype>
#include <algorithm>


using std::size_t;

const int VeryLongInt::BASE = 268435456;   // 2^28
const int VeryLongInt::LOGB = 28;          // log2(BASE);


// Default constructor
// Creates a VeryLongInt object with the initial value 0.
VeryLongInt::VeryLongInt() : 
    VeryLongInt(0ULL) {}


// Copy constructor
VeryLongInt::VeryLongInt(const VeryLongInt& x) : 
    digits(x.digits) {}


// Move constructor
VeryLongInt::VeryLongInt(VeryLongInt&& x) : 
    digits(std::move(x.digits)) {}


// Constructors for integral types
// Creates a VeryLongInt object with the initial value x.
VeryLongInt::VeryLongInt(short x) : 
    VeryLongInt(static_cast<unsigned long long>(x)) {}

VeryLongInt::VeryLongInt(int x) : 
    VeryLongInt(static_cast<unsigned long long>(x)) {}

VeryLongInt::VeryLongInt(long x) : 
    VeryLongInt(static_cast<unsigned long long>(x)) {}

VeryLongInt::VeryLongInt(long long x) : 
    VeryLongInt(static_cast<unsigned long long>(x)) {}

VeryLongInt::VeryLongInt(unsigned short x) : 
    VeryLongInt(static_cast<unsigned long long>(x)) {}

VeryLongInt::VeryLongInt(unsigned int x) : 
    VeryLongInt(static_cast<unsigned long long>(x)) {}

VeryLongInt::VeryLongInt(unsigned long x) : 
    VeryLongInt(static_cast<unsigned long long>(x)) {}

VeryLongInt::VeryLongInt(unsigned long long x) {
    do digits.push_back(static_cast<digit_t>(x % BASE));
    while (x /= BASE);
}


// Creates a VeryLongInt object, with the value of the 
// 10-base number supplied as a std::string.
VeryLongInt::VeryLongInt(const std::string& str)
{
    if (str.empty())
        return;

    for (const char& c : str)
        if (!std::isdigit(c))
            return;

    digits.push_back(0);
    auto sit = str.begin();
    while (sit != str.end()) {
        for (digit_t& digit : digits)
            digit *= 10;
        digits.front() += *sit - '0';
        align();
        sit++;
    }
}


// Creates a VeryLongInt object, with the value of the 
// 10-base number supplied as a C-style string.
VeryLongInt::VeryLongInt(const char* str) : 
    VeryLongInt(str ? std::string(str) : std::string()) {}


// Moves an overflow occurred after some arithmetic 
// operations to the more significant digits.
void VeryLongInt::align()
{
    digit_t cr = 0;
    auto it = digits.begin();
    while (it != digits.end()) {
        *it += cr;
        cr = *it >> LOGB;
        *it &= ~- BASE;
        it++;
    }
    if (cr) digits.push_back(cr);
}


// Moves the specified amount of BASE digits to the left. 
// Zeros are shifted in from the right.
void VeryLongInt::lshift(size_t n)
{
    if (!*this) return;
    digits.resize(digits.size() + n);
    for (size_t i = digits.size(); i-- > n;)
        digits[i] = digits[i - n];
    for (size_t i = n; i-- > 0;)
        digits[i] = 0;
}


// Moves the specified amount of BASE digits to the right. 
// Excess digits shifted off to the right are discarded.
void VeryLongInt::rshift(size_t n) 
{
    if (!*this) return;
    n = std::min(n, digits.size());
    digits.erase(digits.begin(), digits.begin() + n);
    if (digits.empty()) digits.push_back(0);
}


// Copy assignment operator [=]
// Copies the resources held by the right operand into the 
// left operand.
VeryLongInt& VeryLongInt::operator =(const VeryLongInt& that) 
{
    digits = that.digits;
    return *this;
}


// Move assignment operator [=]
// Moves the resources held by the right operand to the 
// left operand and leave the right operand in NaN state.
VeryLongInt& VeryLongInt::operator =(VeryLongInt&& that) 
{
    if (this == &that) 
        return *this;

    digits = std::move(that.digits);
    that.digits.clear();
    return *this;
}


// Addition assignment operator [+=]
// Adds the value of the right operand to value of *this and 
// assigns the result to it. Produces NaN if any of the 
// operands is NaN.
VeryLongInt& VeryLongInt::operator +=(const VeryLongInt& that) 
{
    if (!isValid())
        return *this;

    if (!that.isValid())
        return *this = NaN();

    size_t n = std::max(digits.size(), that.digits.size());
    digits.resize(n, 0);

    for (size_t i = 0; i < that.digits.size(); i++)
        digits[i] += that.digits[i];
    align();
    return *this;
}


// Subtraction assignment operator [-=]
// Subtracts the value of the right operand from value of 
// *this and assigns the result to it. Produces NaN if the 
// difference is negative or any of the operands is NaN.
VeryLongInt& VeryLongInt::operator -=(const VeryLongInt& that) 
{
    if (!isValid())
        return *this;

    if (!that.isValid())
        return *this = NaN();

    if (*this < that)
        return *this = NaN();

    for (size_t i = 0; i < that.digits.size(); i++)
        digits[i] -= that.digits[i];

    for (size_t i = 0; i < digits.size() - 1; i++) {
        if (digits[i] < 0) {
            digits[i] += BASE;
            digits[i + 1]--;
        }
    }
    while (digits.size() > 1 && !digits.back())
        digits.pop_back();
    return *this;
}


// Multiplication assignment operator [*=]
// Multiplies the value of *this by the value of the right 
// operand and assigns the result to *this. Produces NaN 
// if any of the operands is NaN.
VeryLongInt& VeryLongInt::operator *=(const VeryLongInt& that) 
{
    if (!isValid())
        return *this;

    if (!that.isValid())
        return *this = NaN();

    VeryLongInt product;
    size_t n = digits.size() + that.digits.size() - 1;
    product.digits.resize(n, 0);

    for (size_t i = 0; i < digits.size(); i++) {
        for (size_t j = 0; j < that.digits.size(); j++)
            product.digits[i + j] += digits[i] * that.digits[j];
        product.align();
    }
    *this = std::move(product);
    return *this;
}


// Division assignment operator [/=]
// Divides the value of *this by the value of the right 
// operand and assigns the result to *this. Produces NaN 
// if the divisor is zero or any of the operands is NaN.
VeryLongInt& VeryLongInt::operator /=(const VeryLongInt& that) 
{
    if (!isValid())
        return *this;

    if (!that.isValid() || that == 0)
        return *this = NaN();

    VeryLongInt quotient, x, y;
    size_t n = 1;
    if (digits.size() > that.digits.size())
        n = digits.size() - that.digits.size() + 1;
    quotient.digits.resize(n, 0);

    for (size_t i = n; i-- > 0;) {
        digit_t l = 0, r = ~-BASE;
        while (l < r) {
            digit_t m = (l + r + 1) >> 1;
            y = that * m;
            y.lshift(i);
            if (*this < x + y) r = m - 1;
            else l = m;
        }
        y = that * l;
        y.lshift(i);
        x += y;
        quotient.digits[i] = l;
    }
    while (quotient.digits.size() > 1 && !quotient.digits.back())
        quotient.digits.pop_back();
    *this = std::move(quotient);
    return *this;
}


// Modulo assignment operator [%=]
// Divides the value of *this by the value of the right 
// operand and assigns the remainder to *this. Produces NaN 
// if the divisor is zero or any of the operands is NaN.
VeryLongInt& VeryLongInt::operator %=(const VeryLongInt& that) 
{
    if (!isValid())
        return *this;

    if (!that.isValid() || that == 0)
        return *this = NaN();

    VeryLongInt x = *this;
    x /= that;
    x *= that;
    *this -= x;
    return *this;
}


// Bitwise right shift assignment operator [>>=]
// Moves the specified amount of bits of *this value to the 
// right and assigns the result to it. Excess bits shifted 
// off to the right are discarded.
VeryLongInt& VeryLongInt::operator >>=(unsigned int n) 
{
    if (!isValid())
        return *this;

    rshift(n / LOGB);
    if (!(n %= LOGB))
        return *this;

    digit_t cr = 0;
    auto it = digits.rbegin();
    while (it != digits.rend()) {
        cr = (cr << LOGB) + *it;
        *it = cr >> n;
        cr &= ~-(1LL << n);
        it++;
    }

    if (digits.size() > 1 && !digits.back()) 
        digits.pop_back();
    return *this;
}


// Bitwise left shift assignment operator [<<=]
// Moves the specified amount of bits of *this value to the 
// left and assigns the result to the variable. Zero bits 
// are shifted in from the right.
VeryLongInt& VeryLongInt::operator <<=(unsigned int n) 
{
    if (!isValid())
        return *this;

    lshift(n / LOGB);
    if (!(n %= LOGB)) 
        return *this;

    for (digit_t& digit : digits)
        digit <<= n;

    digit_t cr = 0;
    auto it = digits.begin();
    while (it != digits.end()) {
        *it += cr;
        cr = *it >> LOGB;
        *it &= ~-BASE;
        it++;
    }

    if (cr) digits.push_back(cr);
    return *this;
}


// Number of binary digits
// Returns the number of digits in the binary representation 
// of the value of *this object.
size_t VeryLongInt::numberOfBinaryDigits() const 
{
    if (!isValid()) return 0;
    size_t cnt = (digits.size() - 1) * LOGB;
    digit_t last = digits.back();
    do cnt++; while (last >>= 1);
    return cnt;
}


// Conversion operator (bool)
// Returns true for a valid non-zero values of *this.
VeryLongInt::operator bool() const 
{
    if (!digits.empty())
        return digits.back() != 0;
    return false;
}


// Test validation
// Returns true if the value of *this is a valid number.
// Returns false in the case of NaN value.
bool VeryLongInt::isValid() const 
{
    return !digits.empty();
}


// Equality operator [==]
// Returns true if the operands are valid numbers and are 
// equal. If any of the operands is NaN returns false.
bool operator ==(const VeryLongInt& lhs, const VeryLongInt& rhs)
{
    if (!lhs.isValid() || !rhs.isValid())
        return false;

    if (lhs.digits.size() != rhs.digits.size())
        return false;

    for (size_t i = 0; i < lhs.digits.size(); i++)
        if (lhs.digits[i] != rhs.digits[i])
            return false;

    return true;
}


// Not equal to operator [!=]
// Returns true if the operands are valid numbers and are 
// not equal. If any of the operands is NaN returns false.
bool operator !=(const VeryLongInt& lhs, const VeryLongInt& rhs) 
{
    if (!lhs.isValid() || !rhs.isValid())
        return false;

    return !(lhs == rhs);
}


// Less than operator [<]
// Returns true if the operands are valid numbers and the 
// left operand is less than the right operand. If any of 
// the operands is NaN returns false.
bool operator <(const VeryLongInt& lhs, const VeryLongInt& rhs)
{
    if (!lhs.isValid() || !rhs.isValid())
        return false;

    if (lhs.digits.size() != rhs.digits.size())
        return lhs.digits.size() < rhs.digits.size();

    size_t i = lhs.digits.size() - 1;
    while (i && lhs.digits[i] == rhs.digits[i]) i--;
    return lhs.digits[i] < rhs.digits[i];
}


// Greater than operator [>]
// Returns true if the operands are valid numbers and the 
// left operand is greater than the right operand. If any 
// of the operands is NaN returns false.
bool operator >(const VeryLongInt& lhs, const VeryLongInt& rhs)
{
    if (!lhs.isValid() || !rhs.isValid())
        return false;

    return (rhs < lhs);
}


// Less than or equal to operator [<=]
// Returns true if the operands are valid numbers and the 
// left operand is less than or equal to the right operand. 
// If any of the operands is NaN returns false.
bool operator <=(const VeryLongInt& lhs, const VeryLongInt& rhs)
{
    if (!lhs.isValid() || !rhs.isValid())
        return false;

    return !(lhs > rhs);
}


// Greater than or equal to operator [>=]
// Returns true if the operands are valid numbers and the 
// left operand is greater than or equal to the right 
// operand. If any of the operands is NaN returns false.
bool operator >=(const VeryLongInt& lhs, const VeryLongInt& rhs)
{
    if (!lhs.isValid() || !rhs.isValid())
        return false;

    return !(lhs < rhs);
}


// Addition operator [+]
// Returns the sum of the operands. Returns NaN if any 
// of the operands is NaN.
const VeryLongInt operator +(const VeryLongInt& lhs, const VeryLongInt& rhs)
{
    VeryLongInt z = lhs;
    z += rhs;
    return z;
}


// Subtraction operator [-]
// Returns the difference of the operands where the left 
// operand is the minuend and the right operand is the 
// subtrahend. Returns NaN if the result is negative or 
// any of the operands is NaN.
const VeryLongInt operator -(const VeryLongInt& lhs, const VeryLongInt& rhs)
{
    VeryLongInt z = lhs;
    z -= rhs;
    return z;
}


// Multiplication operator [*]
// Returns the product of the operands. Returns NaN if any 
// of the operands is NaN.
const VeryLongInt operator *(const VeryLongInt& lhs, const VeryLongInt& rhs)
{
    VeryLongInt z = lhs;
    z *= rhs;
    return z;
}


// Division operator [/]
// Returns the integer quotient of the operands where the 
// left operand is the dividend and the right operand is 
// the divisor. Returns NaN if the divisor is zero or any 
// of the operands is NaN.
const VeryLongInt operator /(const VeryLongInt& lhs, const VeryLongInt& rhs)
{
    VeryLongInt z = lhs;
    z /= rhs;
    return z;
}


// Modulo operator [%]
// Returns the remainder left over when the left operand 
// is divided by the right operand. Returns NaN if the 
// divisor is zero or any of the operands is NaN.
const VeryLongInt operator %(const VeryLongInt& lhs, const VeryLongInt& rhs)
{
    VeryLongInt z = lhs;
    z %= rhs;
    return z;
}


// Bitwise right shift operator [>>]
// Returns the value of the first operand shifted by the 
// specified number of bits to the right. Excess bits 
// shifted off to the right are discarded. Zero bits are 
// shifted in from the left. 
const VeryLongInt operator >>(const VeryLongInt& lhs, unsigned int rhs)
{
    VeryLongInt w = lhs;
    w >>= rhs;
    return w;
}


// Bitwise left shift operator [<<]
// Returns the value of the first operand shifted by the 
// specified number of bits to the left. Zero bits are 
// shifted in from the right.
const VeryLongInt operator <<(const VeryLongInt& lhs, unsigned int rhs)
{
    VeryLongInt w = lhs;
    w <<= rhs;
    return w; 
}


// Output stream insertion operator [<<]
// Inserts into the output stream a sequence of characters 
// with the 10-base representation of the right operand.
std::ostream& operator <<(std::ostream& os, const VeryLongInt& x) 
{
    if (!x.isValid())
        return os << "NaN";

    using digit_t = VeryLongInt::digit_t;

    static const unsigned DEC_BASE = 1000000000U;   // 10^9
    static const unsigned DEC_LOGB = 9U;            // log10(DEC_BASE)

    const std::vector<digit_t>& bin_digits = x.digits;
    std::vector<digit_t> dec_digits;
    dec_digits.push_back(0);

    auto bit = bin_digits.rbegin();
    while (bit != bin_digits.rend()) {
        for (digit_t& dec_digit : dec_digits)
            dec_digit <<= VeryLongInt::LOGB;
        dec_digits.front() += *bit;
        bit++;

        digit_t cr = 0;
        auto dit = dec_digits.begin();
        while (dit != dec_digits.end()) {
            *dit += cr;
            cr = *dit / DEC_BASE;
            *dit %= DEC_BASE;
            dit++;
        }
        if (cr) dec_digits.push_back(cr);
    }

    auto dit = dec_digits.rbegin();
    os << *dit;
    while (++dit != dec_digits.rend()) 
        os << std::setw(DEC_LOGB) << std::setfill('0') << *dit;

    return os;
}


// Returns a reference to the unmodifiable NaN object.
const VeryLongInt& NaN() 
{
    static const VeryLongInt nan_instance("NAN");
    return nan_instance;
}


// Returns a reference to the unmodifiable zero object.
const VeryLongInt& Zero() 
{
    static const VeryLongInt zero_instance(0ULL);
    return zero_instance;
}


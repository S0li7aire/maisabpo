#include "bigInteger.h"
BigInteger::BigInteger(const std::string& str) 
{
    int start = 0;
    negative = false;

    if (str[0] == '-') {
        negative = true;
        start = 1;
    }

    for (int i = str.length() - 1; i >= start; i--) {
        digits.push_back(str[i] - '0');
    }
}

BigInteger::BigInteger(const std::vector<int>& digits)
{
    this->digits = digits;
    this->negative = false;
}

// inline BigInteger BigInteger::operator=(const BigInteger& other) const
// {
//     BigInteger tmp;
//     tmp.digits = other.digits;
//     tmp.negative = other.negative;
//     return tmp;
// }

inline BigInteger BigInteger::operator+(const BigInteger& other) const 
{
    BigInteger result;
    result.negative = false;

    if(this->negative && !other.negative)
    {
        BigInteger temp = *this;
        temp.negative = false;
        result = other - temp;
        return result;
    }

    if(!this->negative && other.negative)
    {
        BigInteger temp = other;
        temp.negative = false;
        result = *this - temp;
        return result;
    }
    BigInteger larger = *this;
    BigInteger smaller = other;
    larger.negative = false;
    smaller.negative = false;
    if(larger < smaller) { std::swap(larger, smaller); }

    int carry = 0;

    for (size_t i = 0; i < larger.digits.size() || carry != 0; ++i) {
        int sum = carry;
        if(i < larger.digits.size()) { sum += larger.digits[i]; }
        if (i < smaller.digits.size()) { sum += smaller.digits[i]; }
        carry = 0;
        result.digits.push_back(sum % 10);
        carry = sum / 10;
    }

    if (carry > 0) {
        result.digits.push_back(carry);
    }
    result.negative = this->negative || other.negative;
    result.removeLeadingZeros();

    return result;
}

inline BigInteger BigInteger::operator+=(const BigInteger& other) const
{
    return *this + other;
}

inline BigInteger BigInteger::operator-(const BigInteger& other) const 
{
    BigInteger result;
    result.negative = false;

    if (negative != other.negative) {
        BigInteger temp = other;
        temp.negative = negative;
        result = *this + temp;
        return result;
    }

    bool swapOperands = *this < other;

    std::vector<int> resultDigits;
    resultDigits.reserve(std::max(digits.size(), other.digits.size()));
    int borrow = 0;

    for(int i = 0; i < std::max(digits.size(), other.digits.size()) || borrow; ++i)
    {
        int a = 0, b = 0;
        if(i > this->digits.size() - 1) { a = 0; }
        else { a = this->digits[i]; }
        if(i > other.digits.size() - 1) { b = 0; }
        else { b = other.digits[i]; }
        if(swapOperands)
        {
            std::swap(a, b);
        }
        int diff = a - b - borrow;
        if (diff < 0) {
            borrow = 0;
            while(diff < 0)
            {
                diff += 10;
                borrow += 1;
            }
        } else { borrow = 0; }
        resultDigits.push_back(diff);
    }
    result.digits = resultDigits;
    result.removeLeadingZeros();

    return result;
}

inline BigInteger BigInteger::operator-=(const BigInteger& other) const
{
    return *this - other;
}

BigInteger BigInteger::operator*(const BigInteger& other) const 
{
    int n = digits.size();
    int m = other.digits.size();

    // If either operand is zero, return zero
    if (n == 1 && digits[0] == 0) {
        return BigInteger("0");
    }
    if (m == 1 && other.digits[0] == 0) {
        return BigInteger("0");
    }

    // Choose a base case for small inputs
    if (n < 50 || m < 50) {
        BigInteger result;
        result.negative = (negative != other.negative);
        result.digits.resize(n + m, 0);

        for (int i = 0; i < n; ++i) {
            int carry = 0;
            for (int j = 0; j < m; ++j) {
                int product = digits[i] * other.digits[j] + result.digits[i + j] + carry;
                result.digits[i + j] = product % 10;
                carry = product / 10;
            }
            result.digits[i + m] += carry;
        }

        result.removeLeadingZeros();

        return result;
    }

    // Split the operands into two parts
    int half = std::max(n, m) / 2;

    BigInteger high1(std::vector<int>(digits.begin(), digits.begin() + std::min(half, n)));
    BigInteger low1(std::vector<int>(digits.begin() + std::min(half, n), digits.end()));

    BigInteger high2(std::vector<int>(other.digits.begin(), other.digits.begin() + std::min(half, m)));
    BigInteger low2(std::vector<int>(other.digits.begin() + std::min(half, m), other.digits.end()));

    // Recursive calls
    BigInteger z0 = low1 * low2;
    BigInteger z1 = (low1 + high1) * (low2 + high2);
    BigInteger z2 = high1 * high2;

    // Combine the results using Karatsuba formula
    BigInteger result = (z2 * BigInteger(std::pow(10, half * 2))) + ((z1 - z2 - z0) * BigInteger(std::pow(10, half))) + z0;

    // Adjust the sign
    result.negative = (negative != other.negative);

    return result;
}

inline BigInteger BigInteger::operator*(const int& other) const
{
    BigInteger rhs = BigInteger(other);
    if(other > 0)
        rhs.negative = false;
    else
        rhs.negative = true;
    return operator*(rhs);
}

inline BigInteger BigInteger::operator*=(const BigInteger& other) const
{
    return *this * other;
}

inline BigInteger BigInteger::operator/(const BigInteger& other) const 
{
    if (other == BigInteger("0")) {
        throw std::invalid_argument("Division by zero is undefined.");
    }

    BigInteger result;
    result.negative = (negative != other.negative);
    BigInteger larger = *this;
    BigInteger smaller = other;
    larger.negative = false;
    smaller.negative = false;
    if(larger < smaller)
    {
        std::swap(larger, smaller);
    }
    bool stop = false;
    while (!stop)
    {
        int offset = larger.digits.size() - smaller.digits.size();
        if(offset > this->digits.size())
        {
            stop = true;
            break;
        }
        int res = 0;
        std::vector<int> tmp;
        for(int i = 0; i < offset; ++i)
        {
            if(larger.digits.size() - 1 < i) { tmp.push_back(0); }
            else { tmp.push_back(larger.digits[larger.digits.size() - 1 - i]); }
        }
        std::reverse(tmp.begin(), tmp.end());
        BigInteger currentDivisor(tmp);
        BigInteger currentDivident = smaller;
        while(currentDivisor < smaller)
        {
            ++offset;
            tmp.clear();
            for(int i = 0; i < offset; ++i)
            {
                if(larger.digits.size() - 1 < i) { tmp.push_back(0); }
                else { tmp.push_back(larger.digits[larger.digits.size() - 1 - i]); }
            }
            std::reverse(tmp.begin(), tmp.end());
            currentDivisor = BigInteger(tmp);
        }
        std::cout << currentDivisor << " - " << smaller << std::endl;
        while(currentDivisor >= currentDivident)
        {
            ++res;
            currentDivident = smaller * BigInteger(res);
            std::cout << currentDivisor << " + " << currentDivident << std::endl;
        }
        result.digits.push_back(res - 1);
        currentDivident = currentDivident - smaller;
        int i = currentDivident == currentDivisor ? 0 : 1;
        std::vector<int> temp(larger.digits.size() - currentDivident.digits.size() - i, 0);
        for(auto& iter : currentDivident.digits)
        {
            temp.push_back(iter);
        }
        std::cout << " T = " << BigInteger(temp);
        std::cout << " B = " << larger;
        larger = larger - BigInteger(temp);
        std::cout << " A = " << larger << std::endl;
        std::cout << res << " - " << result << std::endl;
    }
    return result;
}

inline BigInteger BigInteger::operator/=(const BigInteger& other) const
{
    return *this / other;
}

inline BigInteger BigInteger::operator%(const BigInteger& other) const 
{
    if (other == BigInteger("0")) {
        throw std::invalid_argument("Arithmetic exception");
    }

    return *this - (other * (*this / other));
}

inline BigInteger BigInteger::operator%=(const BigInteger& other) const
{
    return *this % other;
}

inline BigInteger BigInteger::operator<<(int shiftAmount) const 
{
    if (shiftAmount < 0) {
        throw std::invalid_argument("Shift amount must be non-negative.");
    }

    BigInteger result = *this;

    for (int i = 0; i < shiftAmount; i++) {
        result.digits.insert(result.digits.begin(), 0);
    }

    return result;
}

inline BigInteger BigInteger::operator<<=(const int shiftAmount) const
{
    return *this << shiftAmount;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& bigInt) 
{
    if (bigInt.negative) {
        os << "-";
    }
    
    for (int i = bigInt.digits.size() - 1; i >= 0; i--) {
        os << bigInt.digits[i];
    }
    
    return os;
}

inline bool BigInteger::operator==(const BigInteger& other) const 
{
    if (negative != other.negative) {
        return false;
    }

    if (digits.size() != other.digits.size()) {
        return false;
    }

    for (size_t i = 0; i < digits.size(); i++) {
        if (digits[i] != other.digits[i]) {
            return false;
        }
    }

    return true;
}

inline bool BigInteger::operator!=(const BigInteger& other) const
{
    return !(*this == other);
}

inline bool BigInteger::operator<(const BigInteger& other) const 
{
    if (negative != other.negative) {
        return negative;
    }

    if (digits.size() != other.digits.size()) {
        return (digits.size() < other.digits.size()) ^ negative;
    }

    for (int i = digits.size() - 1; i >= 0; i--) {
        if (digits[i] != other.digits[i]) {
            return (digits[i] < other.digits[i]) ^ negative;
        }
    }

    return false;
}

inline bool BigInteger::operator>(const BigInteger& other) const 
{
    return !(operator<(other) || operator==(other));
}

inline bool BigInteger::operator<=(const BigInteger& other) const 
{
    return operator<(other) || operator==(other);
}

inline bool BigInteger::operator>=(const BigInteger& other) const
{
    return (operator>(other) || operator==(other));
}

void BigInteger::removeLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
}

void BigInteger::print() const {
    if (negative) {
        std::cout << "-";
    }
    for (int i = digits.size() - 1; i >= 0; i--) {
        std::cout << digits[i];
    }
    std::cout << std::endl;
}

int main() {
    BigInteger num1("-123456789123456789");
    BigInteger num2("987654321");

    BigInteger sum = num1 + num2;
    BigInteger diff = num1 - num2;
    BigInteger product = num1 * num2;
    BigInteger quotient = num1 / num2;
    // BigInteger modulus = num1 % num2;
    
    std::cout << "Sum: " << sum << std::endl;

    std::cout << "Diff: " << diff << std::endl;
    
    std::cout << "Product: " << product << std::endl;

    std::cout << "Quotient: " << quotient << std::endl;
    
    // std::cout << "Modulus: " << modulus << std::endl;
    
    return 0;
}

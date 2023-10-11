#include "bigInteger.h"

BigInteger num1("-1234567890123456789012345678901234567890");
BigInteger num2("9876543210987654321098765432109876543210");

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

BigInteger::BigInteger(const std::vector<int>& digits, bool negative)
{
    this->digits = digits;
    this->negative = negative;
}

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

    if (n == 1 && digits[0] == 0) {
        return BigInteger("0");
    }
    if (m == 1 && other.digits[0] == 0) {
        return BigInteger("0");
    }

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

    int half = std::max(n, m) / 2;

    BigInteger high1(std::vector<int>(digits.begin(), digits.begin() + std::min(half, n)));
    BigInteger low1(std::vector<int>(digits.begin() + std::min(half, n), digits.end()));

    BigInteger high2(std::vector<int>(other.digits.begin(), other.digits.begin() + std::min(half, m)));
    BigInteger low2(std::vector<int>(other.digits.begin() + std::min(half, m), other.digits.end()));

    BigInteger z0 = low1 * low2;
    BigInteger z1 = (low1 + high1) * (low2 + high2);
    BigInteger z2 = high1 * high2;

    BigInteger result = (z2 * BigInteger(std::pow(10, half * 2))) + ((z1 - z2 - z0) * BigInteger(std::pow(10, half))) + z0;

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

BigInteger BigInteger::operator/(const BigInteger& other) const 
{
    if (other == BigInteger("0")) {
        throw std::invalid_argument("Arithmetic exception");
    }
    BigInteger t1 = *this, t2 = other;
    t1.negative = false;
    t2.negative = false;
    if (t1 < t2) {
        t1 = BigInteger("0");
        return t1;
    }
    std::vector<int> temp;
    std::vector<int>::reverse_iterator iter = t1.digits.rbegin();
    BigInteger temp2(0);
    while (iter != t1.digits.rend()) {
        temp2 = temp2 * BigInteger("10") + BigInteger((int)(*iter));
        int s = 0;
        while (temp2 >= t2) {
            temp2 = temp2 - t2;
            s = s + 1;
        }
        temp.push_back(s);
        iter++;
    }
    t1.digits.clear();
    std::reverse(temp.begin(), temp.end());
    t1.digits = temp;
    t1.removeLeadingZeros();
    t1.negative = (this->negative != other.negative);
    return t1;
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
    BigInteger result;
    result.negative = this->negative != other.negative;
    auto tempT = *this;
    auto tempO = other;
    tempT.negative = false;
    tempO.negative = false;
    auto tmp = tempT - (tempO * (tempT / tempO));
    result.digits = tmp.digits;
    return result;
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

inline void summ(BigInteger num1, BigInteger num2)
{
    BigInteger num3 = num1 + num2;
}

inline void mul(BigInteger num1, BigInteger num2)
{
    BigInteger num3 = num1 * num2;
}

inline void sub(BigInteger num1, BigInteger num2)
{
    BigInteger num3 = num1 - num2;
}

inline void div(BigInteger num1, BigInteger num2)
{
    BigInteger num3 = num1 / num2;
}

void Addition_BigInteger(benchmark::State& state) {
    while (state.KeepRunning()) {
        summ(num1, num2);
    }
}
BENCHMARK(Addition_BigInteger)->Arg(100000);

void Multiplication_BigInteger(benchmark::State& state) {
    while (state.KeepRunning()) {
        mul(num1, num2);
    }
}
BENCHMARK(Multiplication_BigInteger)->Arg(100000);

void Substraction_BigInteger(benchmark::State& state) {
    while (state.KeepRunning()) {
        sub(num1, num2);
    }
}
BENCHMARK(Substraction_BigInteger)->Arg(100000);

void Division_BigInteger(benchmark::State& state) {
    while (state.KeepRunning()) {
        div(num1, num2);
    }
}
BENCHMARK(Division_BigInteger)->Arg(100000);

BENCHMARK_MAIN();

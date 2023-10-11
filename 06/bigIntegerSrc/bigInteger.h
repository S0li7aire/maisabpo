#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <benchmark/benchmark.h>

class BigInteger {
private:
    std::vector<int> digits;
    bool negative = false;;

public:
    explicit BigInteger() : digits(0), negative(false) {}
    explicit BigInteger(const std::string& str);
    explicit BigInteger(const std::vector<int>& digits, bool negative = false);
    explicit BigInteger(const int num) { 
            digits.push_back(num); 
            num >= 0 ? this->negative = false : this->negative = true;
        }
    ~BigInteger() = default;

public:
    inline BigInteger operator+(const BigInteger& other) const;
    inline BigInteger operator+=(const BigInteger& other) const;
    inline BigInteger operator-(const BigInteger& other) const;
    inline BigInteger operator-=(const BigInteger& other) const;
    BigInteger operator*(const BigInteger& other) const;
    BigInteger operator*(const int& other) const;
    inline BigInteger operator*=(const BigInteger& other) const;
    BigInteger operator/(const BigInteger& other) const;
    inline BigInteger operator/=(const BigInteger& other) const;
    inline BigInteger operator%(const BigInteger& other) const;
    inline BigInteger operator%=(const BigInteger& other) const;
    inline BigInteger operator<<(const int shiftAmount) const;
    inline BigInteger operator<<=(const int shiftAmount) const;
    inline bool operator==(const BigInteger& other) const;
    inline bool operator!=(const BigInteger& other) const;
    inline bool operator<(const BigInteger& other) const;
    inline bool operator>(const BigInteger& other) const;
    inline bool operator>=(const BigInteger& other) const;
    inline bool operator<=(const BigInteger& other) const;
    friend std::ostream& operator<<(std::ostream& os, const BigInteger& bigInt);

public:
    void print() const;

private:
    void removeLeadingZeros();
};
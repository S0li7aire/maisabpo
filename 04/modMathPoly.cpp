#include <iostream>
#include <cstdint>
#include <conio.h>

namespace modMathPoly
{
    template<typename T>
    inline T addPolynomials(T a, T b, T M) { return (a ^ b) % M; }
    
    template<typename T>
    inline T subtractPolynomials(T a, T b, T M) { return (a ^ b) % M; }

    template<typename T>
    inline T multiplyPolynomials(T a, T b, T M) 
    {
        T result = 0;
        while (b > 0) {
            if (b & 1)
                result = addPolynomials(result, a, M);
            a <<= 1;

            if (a >= M)
                a = subtractPolynomials(a, M, M);
            b >>= 1;
        }
        return result;
    }

    template<typename T>
    inline T findInverse(T a, T M) 
    {
        int t = 0, newt = 1,
            r = M, newr = a;

        while (newr != 0) {
            int quotient = r / newr;
            int tmp = newt;
            newt = t - quotient * newt;
            t = tmp;

            tmp = newr;
            newr = r - quotient * newr;
            r = tmp;
        }

        if (r > 1) return 0;

        if (t < 0)  t += M;

        return t;
    }
}

namespace utility
{
    void printBinary(int number) {
        bool flag = false;
        for (int i = sizeof(int) * 8 - 1; i >= 0; i--) {
            int bit = (number >> i) & 1;
            if(bit == 1 || flag)
            {
                std::cout << bit;
                flag = true;
            }
        }
        std::cout << std::endl;
    }

    void readWord(uint32_t &var)
    {
        char ch = 0;
        while ((ch = _getch()) != '\r' && ch != '\n') {
            if(ch == '\b')
            {
                var /= 10;
                std::cout << "\b \b";
                continue;
            }
            var = var * 10 + (ch - '0');
            std::cout << ch;
        }
    }
}


int main() {
    uint32_t a = 0,
             b = 0,
             M = 0;

    std::cout << "Введите значение a: ";
    utility::readWord(a);
    std::cout << " = ";
    utility::printBinary(a);

    std::cout << "Введите значение b: ";
    utility::readWord(b);
    std::cout << " = ";
    utility::printBinary(b);

    std::cout << "Введите значение M (модуль): ";
    utility::readWord(M);
    std::cout << " = ";
    utility::printBinary(M);

    std::cout << "a + b mod M = " << modMathPoly::addPolynomials(a, b, M) << std::endl;
    std::cout << "a - b mod M = " << modMathPoly::subtractPolynomials(a, b, M) << std::endl;
    std::cout << "a * b mod M = " << modMathPoly::multiplyPolynomials(a, b, M) << std::endl;
    std::cout << "2^(-1) mod M = " << modMathPoly::findInverse(uint32_t(2), M) << std::endl;

    return 0;
}

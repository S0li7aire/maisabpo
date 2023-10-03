#include <iostream>

namespace modMath
{
    inline int sum(int a, int b, int M){ return (a + b) % M; }
    inline int diff(int a, int b, int M){ return (a - b + M) % M; }
    inline int mul(int a, int b, int M){ return (a * b) % M; }
    inline double pow(int a, int b, int M)
    { 
        if (M == 1) return 0;
        int result = 1;
        a = a % M;
        while (std::abs(b) > 0) 
        {
            if (std::abs(b % 2) == 1) 
            {
                result = (result * a) % M;
            }
            b = b / 2;
            a = (a * a) % M;
        }
        return b > 0 ? result : (1. / result);
    }
    inline int findInverse(int a, int M)
    {
        for (int i = 1; i < std::abs(M); i++) {
            if (std::abs((a * i) % M) == 1) {
                return a > 0 ? i : -i;
            }
        }
        return NULL;
    }
    inline int div(int a, int b, int M)
    { 
        int B = modMath::findInverse(b, M);
        return B == -1 ? B : (a * B) % M; 
    }
}

int main() {
    int a = 0, 
        b = 0, 
        M = 0;

    std::cout << "Введите значение a: ";
    std::cin >> a;

    std::cout << "Введите значение b: ";
    std::cin >> b;

    std::cout << "Введите значение M (модуль): ";
    std::cin >> M;

    std::cout << "(a + b) mod M = " << modMath::sum(a, b, M) << std::endl;

    std::cout << "(a - b) mod M = " << modMath::diff(a, b, M) << std::endl;

    std::cout << "(a * b) mod M = " << modMath::mul(a, b, M) << std::endl;

    std::cout << "(a ^ b) mod M = " << modMath::pow(a, b, M) << std::endl;

    int temp = modMath::findInverse(a, M);
    if (temp != NULL)
        std::cout << "a^(-1) mod M = " << temp << std::endl;
    else 
        std::cout << "a^(-1) mod M: нет решения" << std::endl;

    temp = modMath::findInverse(b, M);
    if (temp != NULL)
        std::cout << "b^(-1) mod M = " << temp << std::endl;
    else
        std::cout << "b^(-1) mod M: нет решения" << std::endl;

    temp = modMath::div(b, a, M);
    if (temp != NULL)
        std::cout << "(b / a) mod M = " << temp << std::endl;
    else
        std::cout << "(b / a) mod M: нет решения" << std::endl;

    temp = modMath::div(a, b, M);
    if (temp != NULL)
        std::cout << "(a / b) mod M = " << temp << std::endl;
    else
        std::cout << "(a / b) mod M: нет решения" << std::endl;

    return 0;
}

#include <iostream>
#include <iomanip>
#include <vector>

namespace prime
{
    inline std::vector<int> sieveOfEratosthenes(int N) 
    {
        std::vector<bool> isPrime(N + 1, true);
        std::vector<int> primes;
        primes.reserve(N / 4);

        for (int p = 2; p * p <= N; ++p) 
        {
            if (isPrime[p])
                for (int i = p * p; i <= N; i += p)
                    isPrime[i] = false;
        }

        for (int p = 2; p <= N; ++p) 
        {
            if (isPrime[p]) { primes.push_back(p); }
        }

        return primes;
    }

    inline bool isPrime(int num) 
    {
        if (num <= 1) { return false; }
        if (num <= 3) { return true; }
        if (num % 2 == 0 || num % 3 == 0) { return false; }

        for (int i = 5; i * i <= num; i += 6)
            if (num % i == 0 || num % (i + 2) == 0)
                return false;

        return true;
    }
}

namespace utility
{
    inline int countDigits(int number) 
    {
        int count = 0;
        
        while (number != 0) 
        {
            number /= 10;
            count++;
        }
        
        return count;
    }
}

int main() 
{
    int N;
    std::cout << "Введите число N: ";
    std::cin >> N;

    std::vector<int> primes = prime::sieveOfEratosthenes(N);
    std::cout << "Первые " << N << " простых чисел: " << std::endl;
    int count = 0;
    for (int prime : primes) 
    {
        std::cout << std::setw(utility::countDigits(N)) << prime << " ";
        ++count;
        if(count == 10)
        {
            std::cout << std::endl;
            count = 0;
        }
    }
    std::cout << std::endl;

    std::cout << "Введите число для проверки на простоту: ";
    std::cin >> N;
    auto c = prime::isPrime(N) ? " - простое число." : " - не является простым числом.";
    std::cout << N << c << std::endl;

    return 0;
}

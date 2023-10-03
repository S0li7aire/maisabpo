#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <cmath>

namespace bitwise
{
    void printRes(const std::string type, const int num_1, const int num_2)
    {
        int numBits = sizeof(num_2) * 8;
        int res = 0;
        if (type == "xor") 
        {
            res = num_1 ^ num_2;
        }
        else if (type == "and") 
        {
            res = num_1 & num_2;
        }
        else if (type == "or") 
        {
            res = num_1 | num_2;
        }
        else if (type == "set1") 
        {
            res = num_2 | (1 << num_1);
        }
        else if (type == "set0") 
        {
            res = num_2 & ~(1 << num_1);
        }
        else if (type == "shl") 
        {
            res = num_2;
            res = res << num_1;
        }
        else if (type == "shr") 
        {
            res = num_2;
            res = res >> num_1;
        }
        else if (type == "rol") 
        {
            int number1 = num_1 % numBits;
            int number2 = num_2;
            unsigned int shiftedBits = (number2 >> (numBits - number1)) & static_cast<unsigned int>((1 << number1) - 1);
            number2 <<= number1;
            res = number2 | shiftedBits;
        }
        else if (type == "ror") 
        {
            int number1 = num_1 % numBits;
            int number2 = num_2;
            unsigned int shiftedBits = number2 & static_cast<unsigned int>((1 << number1) - 1);
            number2 >>= number1;
            res = number2 | (shiftedBits << (numBits - number1));
        }
        else if (type == "mix") 
        {
            std::vector<bool> rNumber;
            rNumber.reserve(8);
            int num1 = num_1;
            for(int i = 7; i >= 0; --i)
            {
                rNumber[num1 % 10] = (num_2 >> i) & 1;
                num1 /= 10;
            }
            for(int i = 0; i < 8; ++i)
                res += rNumber[i] * std::pow(2, i);
        }
        else
            return;
        std::cout << res;
    }
}

int main(int argc, const char* argv[])
{
    std::string com = argv[1];
    int num_1 = stoi(static_cast<std::string>(argv[2]));
    int num_2 = stoi(static_cast<std::string>(argv[3]));
    bitwise::printRes(com, num_1, num_2);
    return 0;
}
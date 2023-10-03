#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

namespace binOut
{
    class binaryRep
    {
    public:
        binaryRep() = default;
        ~binaryRep() = default;
        void printData(const char* argv);
        inline void setData(std::ifstream& data){ data_ = std::vector<unsigned char>(std::istreambuf_iterator<char>(data), {}); }
    private:
        std::vector<unsigned char> data_;
    };

    void binaryRep::printData(const char* argv)
    {
        std::string data = static_cast<std::string>(argv);
        if (data == "hex8") 
        {
            for (const auto& byte : data_) {
                std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte) << " ";
            }
            std::cout << std::dec << std::endl;
        }
        else if (data == "dec8") 
        {
            for (const auto& byte : data_) {
                std::cout << std::setw(3) << std::setfill('0') << static_cast<int>(byte) << " ";
            }
            std::cout << std::endl;
        }
        else if (data == "hex16") 
        {
            for (size_t i = 0; i < data_.size(); i += 2) {
                unsigned short value = data_[i] | (data_[i + 1] << 8);
                std::cout << std::setw(4) << std::setfill('0') << std::hex << value << " ";
            }
        }
        else if (data == "dec16") 
        {
            for (size_t i = 0; i < data_.size(); i += 2) {
                unsigned short value = data_[i] | (data_[i + 1] << 8);
                std::cout << std::setw(5) << std::setfill('0') << std::dec << value << " ";
            }
            std::cout << std::endl;
        }
        else if (data == "hex32") 
        {
            for (size_t i = 0; i < data_.size(); i += 4) {
                unsigned int value = data_[i] | (data_[i + 1] << 8) | (data_[i + 2] << 16) | (data_[i + 3] << 24);
                std::cout << std::setw(8) << std::setfill('0') << std::hex << value << " ";
            }
            std::cout << std::dec << std::endl;
        }
        else
            return;
    }
}

int main(int argc, const char* argv[])
{
    binOut::binaryRep *file = new binOut::binaryRep();
    std::string path = argv[2];
    std::ifstream finput; 
    finput.open(path, std::ios::binary);
    if(!finput)
        return 1;
    file->setData(finput);
    file->printData(argv[1]);
    return 0;
}
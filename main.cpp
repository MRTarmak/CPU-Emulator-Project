#include <iostream>
#include <fstream>
#include "CPULib/cpulib.hpp"

using namespace CPULib;

int main()
{
    std::string path;
    std::cout << "Enter path: ";
    std::cin >> path;

    std::ifstream file;
    file.open(path);
    if (!file.is_open())
        std::cerr << "Error: can not open a file with this path" << std::endl;

    std::string command;
    while (file >> command)
    {
        std::cout << command << std::endl;
    }

    return 0;
}

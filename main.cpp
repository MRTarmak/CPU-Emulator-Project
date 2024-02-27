#include <iostream>
#include "CPULib/cpulib.hpp"

using namespace CPULib;

int main()
{
    stack<int> s;
    int n = 10;
    std::cout << s.empty() << std::endl;
    s.push(n);
    std::cout << s.top() << std::endl;
    s.pop();
    std::cout << s.empty() << std::endl;

    return 0;
}

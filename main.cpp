#include <iostream>
#include "CPULib/cpulib.hpp"

using namespace CPULib;

int main()
{
    cpu<long long> cpu_1;
    cpu_1.get_program();
    cpu_1.start();

    return 0;
}

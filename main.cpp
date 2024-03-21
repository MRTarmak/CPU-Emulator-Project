#include "CPULib/cpulib.hpp"

using namespace CPULib;

int main()
{
    cpu<long long> cpu_1;
    std::string console_command;
    while (true)
    {
        std::cin >> console_command;

        if (console_command == "kill")
        {
            break;
        }
        else if (console_command == "parse")
        {
            cpu_1.launch_parser();
        }
        else if (console_command == "start")
        {
            cpu_1.start();
        }
        else
        {
            std::cout << "Unknown command" << std::endl;
            continue;
        }
    }

    return 0;
}

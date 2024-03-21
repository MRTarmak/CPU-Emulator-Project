#pragma once

#include <vector>
#include <fstream>

namespace CPULib
{
    class preprocessor
    {
    public:
        void get_file()
        {
            std::string path;
            std::cout << "Enter path: ";
            std::cin >> path;
            path = "../" + path;

            file.open(path);
            if (!file.is_open())
            {
                std::cerr << "Error: Can not open a file with given path" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

    protected:
        std::ifstream file;
    };

    class parser : preprocessor
    {
    public:
        void get_program()
        {
            get_file();
            std::string command;
            while (file >> command)
            {
                if (command == "BEGIN")
                {
                    program.push_back({11});
                    PC++;
                }
                else if (command == "END")
                {
                    program.push_back({12});
                    PC++;
                }
                else if (command == "PUSH")
                {
                    file >> command;
                    long long number = 0;
                    size_t length = command.length();
                    for (int i = 0; i < length; i++)
                    {
                        if (command[i] < '0' || command[i] > '9')
                        {
                            std::cerr << "Error: value0 must be a number" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        else
                        {
                            number *= 10;
                            number += (command[i] - '0');
                        }
                    }
                    program.push_back({13, number});
                    PC++;
                }
                else if (command == "POP")
                {
                    program.push_back({14});
                    PC++;
                }
                else if (command == "PUSHR" || command == "POPR")
                {
                    long long cmd;
                    if (command == "PUSHR")
                        cmd = 15;
                    else
                        cmd = 16;

                    file >> command;
                    if (command != "AX" && command != "BX" && command != "CX" && command != "DX")
                    {
                        std::cerr << "Error: Unknown register name" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    else if (command == "AX")
                    {
                        program.push_back({cmd, 0});
                    }
                    else if (command == "BX")
                    {
                        program.push_back({cmd, 1});
                    }
                    else if (command == "CX")
                    {
                        program.push_back({cmd, 2});
                    }
                    else if (command == "DX")
                    {
                        program.push_back({cmd, 3});
                    }
                    PC++;
                }
                else if (command == "ADD" || command == "SUB" ||
                         command == "MUL" || command == "DIV")
                {
                    if (command == "ADD")
                        program.push_back({17});
                    else if (command == "SUB")
                        program.push_back({18});
                    else if (command == "MUL")
                        program.push_back({19});
                    else
                        program.push_back({110});
                    PC++;
                }
                else if (command == "OUT" || command == "IN")
                {
                    if (command == "OUT")
                        program.push_back({111});
                    else
                        program.push_back({112});
                    PC++;
                }
                else if (command == "JMP" || command == "JEQ" || command == "JNE" || command == "JA" ||
                         command == "JAE" || command == "JB" || command == "JBE" || command == "CALL")
                {
                    long long cmd;
                    if (command == "JMP")
                        cmd = 113;
                    else if (command == "JEQ")
                        cmd = 114;
                    else if (command == "JNE")
                        cmd = 115;
                    else if (command == "JA")
                        cmd = 116;
                    else if (command == "JAE")
                        cmd = 117;
                    else if (command == "JB")
                        cmd = 118;
                    else if (command == "JBE")
                        cmd = 119;
                    else
                        cmd = 120;

                    file >> command;
                    program.push_back({cmd, static_cast<long long>(labels.size())});

                    labels.push_back(command);
                    labels_ind.push_back(-1);

                    PC++;
                }
                else if (command == "RET")
                {
                    program.push_back({121});
                    PC++;
                }
                else
                {
                    if (command.back() == ':')
                    {
                        command.pop_back();
                        labels.push_back(command);
                        labels_ind.push_back(PC + 1);
                    }
                    else
                    {
                        std::cerr << "Error: Unknown command" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }

        void change_labels()
        {
            int i = 0;
            while (program[i][0] != 12)
            {
                if (program[i][0] >= 113 && program[i][0] <= 120)
                {
                    bool not_found = true;
                    for (int j = 0; j < labels.size(); j++)
                    {
                        if (labels[program[i][1]] == labels[j] && labels_ind[j] != -1)
                        {
                            program[i][1] = labels_ind[j];
                            not_found = false;
                            break;
                        }
                    }
                    if (not_found)
                    {
                        std::cerr << "Error: label is not fount" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                i++;
            }
        }

        void parse()
        {
            get_program();
            change_labels();
        }

    protected:
        std::vector<std::vector<long long>> program;
        std::vector<std::string> labels;
        std::vector<int> labels_ind;
        int PC = -1; // Program Counter register
    };
}
#pragma once

#include <iostream>
#include <vector>
#include <fstream>

namespace CPULib
{
    template<class T>
    class stack
    {
    public:
        // Destructor
        ~stack()
        {
            delete[] arr;
        }

        // Initialization constructor
        stack()
        {
            arr = new T[1];
            len = 0;
            capacity = 0;
        }

        // Copy constructors
        stack(const stack<T> &other)
        {
            *this = other;
        }

        explicit stack(const T &other)
        {
            len = 1;
            capacity = 1;
            arr = new T[1];
            arr[0] = other;
        }

        // Move constructors
        stack(stack<T> &&other) noexcept
        {
            *this = std::move(other);
        }

        explicit stack(T &&other)
        {
            len = 1;
            capacity = 1;
            arr = new T[1];
            arr[0] = std::move(other);
        }

        // Copy assignment operator
        stack<T> &operator=(const stack<T> &other)
        {
            len = other.len;
            capacity = other.capacity;
            arr = new T[len];
            for (int i = 0; i < len; i++)
            {
                arr[i] = other.arr[i];
            }

            return *this;
        }

        // Move assignment operator
        stack<T> &operator=(stack<T> &&other) noexcept
        {
            len = other.len;
            capacity = other.capacity;
            arr = other.arr;
            other.arr = nullptr;
            other.len = 0;

            return *this;
        }

        // Class methods
        void push(const T &value)
        {
            if (len == capacity)
                resize(len);
            arr[len] = value;
            len++;
        }

        void pop()
        {
            if (len != 0)
            {
                len--;
            }
            else
                std::cerr << "Error: Can not pop, stack is empty" << std::endl;
        }

        bool empty()
        {
            return len == 0;
        }

        T &top()
        {
            if (len != 0)
                return arr[len - 1];
            else
                std::cerr << "Error: Can not get top value, stack is empty" << std::endl;
        }

        int get_len()
        {
            return len;
        }

        void set_len(int new_len)
        {
            len = new_len;
        }

    private:
        T *arr = nullptr;
        int len = 0;
        size_t capacity = 0;

        void resize(size_t size)
        {
            auto new_arr = new T[size];
            for (int i = 0; i < len; i++)
            {
                new_arr[i] = std::move(arr[i]);
            }
            delete[] arr;
            arr = new_arr;
            capacity = size;
        }
    };

    template<class T>
    class cpu
    {
    public:
        void get_program()
        {
            std::string path;
            std::cout << "Enter path: ";
            std::cin >> path;
            path = "../" + path;

            std::ifstream file;
            file.open(path);
            if (!file.is_open())
            {
                std::cerr << "Error: Can not open a file with given path" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string command;
            while (file >> command)
            {
                if (command == "BEGIN" || command == "END" || command == "POP" ||
                command == "ADD" || command == "SUB" || command == "MUL" ||
                command == "DIV" || command == "OUT" || command == "IN" ||
                command == "RET")
                {
                    program.push_back(command);
                    PC++;
                }
                else if (command == "PUSH")
                {
                    program.push_back(command);
                    PC++;
                    file >> command;
                    int length = command.length();
                    for (int i = 0; i < length; i++)
                    {
                        if (command[i] < '0' | command[i] > '9')
                        {
                            std::cerr << "Error: value0 must be a number" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                    }
                    program.push_back(command);
                    PC++;
                }
                else if (command == "PUSHR" || command == "POPR")
                {
                    program.push_back(command);
                    PC++;
                    file >> command;
                    if (command != "AX" && command != "BX" && command != "CX" && command != "DX")
                    {
                        std::cerr << "Error: Unknown register name" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    program.push_back(command);
                    PC++;
                }
                else if (command == "JMP" || command == "JEQ" || command == "JNE" || command == "JA" ||
                command == "JAE" || command == "JB" || command == "JBE" || command == "CALL")
                {
                    program.push_back(command);
                    PC++;
                    file >> command;
                    program.push_back(command);
                    PC++;
                }
                else
                {
                    if (command.back() == ':')
                    {
                        command.pop_back();
                        labels.push_back(command);
                        label_ind.push_back(PC + 1);
                        labels_count++;
                    }
                    else
                    {
                        std::cerr << "Error: Unknown command" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }

        void start()
        {
            get_program();
            int end = PC;
            PC = 0;
            while (program[PC] != "BEGIN")
            {
                if (program[PC] == "END" || PC == end)
                {
                    std::cerr << "Error: Program must have a BEGIN command" << std::endl;
                    exit(EXIT_FAILURE);
                }
                PC++;
            }
            PC++;

            while (program[PC] != "END")
            {
                if (PC == end)
                {
                    std::cerr << "Error: Program must have an END command" << std::endl;
                    exit(EXIT_FAILURE);
                }
                if (program[PC] == "PUSH")
                {
                    PC++;
                    T number = 0;
                    int length = program[PC].length();
                    for (int i = 0; i < length; i++)
                    {
                        number *= 10;
                        number += (program[PC][i] - '0');
                    }
                    cpu_stack.push(number);
                    PC++;
                }
                else if (program[PC] == "POP")
                {
                    cpu_stack.pop();
                    PC++;
                }
                else if (program[PC] == "PUSHR")
                {
                    PC++;
                    if (program[PC] == "AX")
                    {
                        if (AX_e)
                        {
                            std::cerr << "Error: Register is empty" << std::endl;
                        }
                        else
                        {
                            cpu_stack.push(AX);
                            AX_e = true;
                        }
                    }
                    else if (program[PC] == "BX")
                    {
                        if (BX_e)
                        {
                            std::cerr << "Error: Register is empty" << std::endl;
                        }
                        else
                        {
                            cpu_stack.push(BX);
                            BX_e = true;
                        }
                    }
                    else if (program[PC] == "CX")
                    {
                        if (CX_e)
                        {
                            std::cerr << "Error: Register is empty" << std::endl;
                        }
                        else
                        {
                            cpu_stack.push(CX);
                            CX_e = true;
                        }
                    }
                    else if (program[PC] == "DX")
                    {
                        if (DX_e)
                        {
                            std::cerr << "Error: Register is empty" << std::endl;
                        }
                        else
                        {
                            cpu_stack.push(DX);
                            DX_e = true;
                        }
                    }
                    PC++;
                }
                else if (program[PC] == "POPR")
                {
                    PC++;
                    if (program[PC] == "AX")
                    {
                        AX = cpu_stack.top();
                        AX_e = false;
                        cpu_stack.pop();
                    }
                    else if (program[PC] == "BX")
                    {
                        BX = cpu_stack.top();
                        BX_e = false;
                        cpu_stack.pop();
                    }
                    else if (program[PC] == "CX")
                    {
                        CX = cpu_stack.top();
                        CX_e = false;
                        cpu_stack.pop();
                    }
                    else if (program[PC] == "DX")
                    {
                        DX = cpu_stack.top();
                        DX_e = false;
                        cpu_stack.pop();
                    }
                    PC++;
                }
                else if (program[PC] == "ADD")
                {
                    OP = cpu_stack.top();
                    cpu_stack.pop();
                    RX = OP + cpu_stack.top();
                    cpu_stack.push(OP);
                    cpu_stack.push(RX);
                    PC++;
                }
                else if (program[PC] == "SUB")
                {
                    OP = cpu_stack.top();
                    cpu_stack.pop();
                    RX = OP - cpu_stack.top();
                    cpu_stack.push(OP);
                    cpu_stack.push(RX);
                    PC++;
                }
                else if (program[PC] == "MUL")
                {
                    OP = cpu_stack.top();
                    cpu_stack.pop();
                    RX = OP * cpu_stack.top();
                    cpu_stack.push(OP);
                    cpu_stack.push(RX);
                    PC++;
                }
                else if (program[PC] == "DIV")
                {
                    OP = cpu_stack.top();
                    cpu_stack.pop();
                    RX = OP * cpu_stack.top();
                    cpu_stack.push(OP);
                    cpu_stack.push(RX);
                    PC++;
                }
                else if (program[PC] == "OUT")
                {
                    std::cout << cpu_stack.top() << std::endl;
                    cpu_stack.pop();
                    PC++;
                }
                else if (program[PC] == "IN")
                {
                    T in;
                    std::cin >> in;
                    cpu_stack.push(in);
                    PC++;
                }
                else if (program[PC] == "JMP")
                {
                    PC++;
                    bool not_found = true;
                    for (int i = 0; i < labels_count; i++)
                    {
                        if (program[PC] == labels[i])
                        {
                            PC = label_ind[i];
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
                else if (program[PC] == "JEQ")
                {
                    OP = cpu_stack.top();
                    cpu_stack.pop();
                    if (OP == cpu_stack.top())
                    {
                        PC++;
                        bool not_found = true;
                        for (int i = 0; i < labels_count; i++)
                        {
                            if (program[PC] == labels[i])
                            {
                                PC = label_ind[i];
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
                    else
                        PC++;
                    cpu_stack.push(OP);
                }
                else if (program[PC] == "JNE")
                {
                    OP = cpu_stack.top();
                    cpu_stack.pop();
                    if (OP != cpu_stack.top())
                    {
                        PC++;
                        bool not_found = true;
                        for (int i = 0; i < labels_count; i++)
                        {
                            if (program[PC] == labels[i])
                            {
                                PC = label_ind[i];
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
                    else
                        PC++;
                    cpu_stack.push(OP);
                }
                else if (program[PC] == "JA")
                {
                    OP = cpu_stack.top();
                    cpu_stack.pop();
                    if (OP > cpu_stack.top())
                    {
                        PC++;
                        bool not_found = true;
                        for (int i = 0; i < labels_count; i++)
                        {
                            if (program[PC] == labels[i])
                            {
                                PC = label_ind[i];
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
                    else
                        PC++;
                    cpu_stack.push(OP);
                }
                else if (program[PC] == "JAE")
                {
                    OP = cpu_stack.top();
                    cpu_stack.pop();
                    if (OP >= cpu_stack.top())
                    {
                        PC++;
                        bool not_found = true;
                        for (int i = 0; i < labels_count; i++)
                        {
                            if (program[PC] == labels[i])
                            {
                                PC = label_ind[i];
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
                    else
                        PC++;
                    cpu_stack.push(OP);
                }
                else if (program[PC] == "JB")
                {
                    OP = cpu_stack.top();
                    cpu_stack.pop();
                    if (OP < cpu_stack.top())
                    {
                        PC++;
                        bool not_found = true;
                        for (int i = 0; i < labels_count; i++)
                        {
                            if (program[PC] == labels[i])
                            {
                                PC = label_ind[i];
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
                    else
                        PC++;
                    cpu_stack.push(OP);
                }
                else if (program[PC] == "JBE")
                {
                    OP = cpu_stack.top();
                    cpu_stack.pop();
                    if (OP <= cpu_stack.top())
                    {
                        PC++;
                        bool not_found = true;
                        for (int i = 0; i < labels_count; i++)
                        {
                            if (program[PC] == labels[i])
                            {
                                PC = label_ind[i];
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
                    else
                        PC++;
                    cpu_stack.push(OP);
                }
                else if (program[PC] == "CALL")
                {
                    PC++;
                    bool not_found = true;
                    for (int i = 0; i < labels_count; i++)
                    {
                        if (program[PC] == labels[i])
                        {
                            program[PC].push_back('@');

                            labels.push_back(program[PC]);
                            label_ind.push_back(PC + 1);
                            labels_count++;

                            func.push(program[PC]);
                            PC = label_ind[i];

                            BP = cpu_stack.get_len();

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
                else if (program[PC] == "RET")
                {
                    for (int i = 0; i < labels_count; i++)
                    {
                        if (func.top() == labels[i])
                        {
                            PC = label_ind[i];
                            func.pop();
                            if (cpu_stack.get_len() > BP)
                                cpu_stack.set_len(BP);
                            break;
                        }
                    }
                }
                else
                {
                    PC++;
                }
            }
        }

    private:
        stack<T> cpu_stack;
        std::vector<std::string> program;
        std::vector<std::string> labels;
        std::vector<int> label_ind;
        int labels_count = 0;
        stack<std::string> func;
        // Registers
        T AX;
        bool AX_e = true;
        T BX;
        bool BX_e = true;
        T CX;
        bool CX_e = true;
        T DX;
        bool DX_e = true;
        T OP; // Operation register
        T RX; // Result register
        int PC = -1; // Program Counter register
        int BP = 0; // Base Pointer register
    };
}
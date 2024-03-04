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
            arr[len] = value;
            len++;
            if (len > capacity)
                resize(len);
        }

        void pop()
        {
            if (!empty())
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
            if (!empty())
                return arr[len - 1];
            else
                std::cerr << "Error: Can not get top value, stack is empty" << std::endl;
        }

    private:
        T *arr = nullptr;
        int len = 0;
        size_t capacity = 0;

        void resize(size_t size)
        {
            T *new_arr = new T[size];
            for (int i = 0; i < len; i++)
            {
                new_arr[i] = arr[i];
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

//            try
//            {
//                file.open(path);
//            }
//            catch (const std::exception &ex)
//            {
//                std::cerr << ex.what() << std::endl;
//            }

            file.open(path);
            if (!file.is_open())
                std::cerr << "Error: Can not open a file with given path" << std::endl;

            std::string command;
            while (file >> command)
            {
                program.push_back(command);
                PC++;
            }
        }

        void start()
        {
            if (program[0] != "BEGIN")
                std::cerr << "Error: Program must start with BEGIN command" << std::endl;
            if (program[PC] != "END")
                std::cerr << "Error: Program must end with END command" << std::endl;

            PC = 1;
            while (program[PC] != "END")
            {
                switch (program[PC])
                {
                    case "PUSH":
                    {
                        PC++;
                        T number = 0;
                        int length = program[PC].length();
                        for (int i = 0; i < length; i++) {
                            if (program[PC][i] < '0' | program[PC][i] > '9')
                                std::cerr << "Error: value0 must be a number" << std::endl;
                            else {
                                number *= 10;
                                number += (program[PC][i] - '0');
                            }
                        }
                        cpu_stack.push(number);
                        PC++;
                        break;
                    }
                    case "POP":
                        cpu_stack.pop();
                        PC++;
                        break;
                }
            }
        }

    private:
        stack<T> cpu_stack;
        std::vector<std::string> program;
        std::vector<std::string> labels;
        std::vector<int> label_ind;
        // Registers
        T AX;
        T BX;
        T CX;
        T DX;
        T PC = -1;
    };
}
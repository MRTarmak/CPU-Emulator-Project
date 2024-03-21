#pragma once

#include <iostream>
#include <stacklib.hpp>
#include <parserlib.hpp>

namespace CPULib
{
    enum class CommandName
    {
        BEGIN = 11,
        END,
        PUSH,
        POP,
        PUSHR,
        POPR,
        ADD,
        SUB,
        MUL,
        DIV = 110,
        OUT,
        IN,
        JMP,
        JEQ,
        JNE,
        JA,
        JAE,
        JB,
        JBE,
        CALL,
        RET
    };

    enum class RegisterName
    {
        OP = 4,
        RX,
        BP
    };

    template <class T>
    struct memory
    {
        stack<T> mem_stack;
        stack<size_t> function;
        std::vector<T> registers = {0, 0, 0, 0, 0, 0, 0};
        std::vector<bool> reg_f = {false, false, false, false};
    };

    template <class T>
    struct command
    {
        virtual void run(memory<T>&){};
        virtual void run(memory<T>&, T&){};
        virtual void run(memory<T>&, T&, T&){};
        virtual void run(memory<T>&, T&, T&, int&){};
        virtual void run(memory<T>&, int&){};
    };

    template <class T>
    struct PUSH : command<T>
    {
        void run(memory<T> &mem, T &arg) override
        {
            mem.mem_stack.push(arg);
        }
    };

    template <class T>
    struct POP : command<T>
    {
        void run(memory<T> &mem) override
        {
            mem.mem_stack.pop();
        }
    };

    template <class T>
    struct PUSHR_POPR : command<T>
    {
        void run(memory<T> &mem, T &arg, T &index) override
        {
            if (arg == static_cast<T>(CommandName::PUSHR))
            {
                if (!mem.reg_f[index])
                {
                    std::cerr << "Error: Register is empty" << std::endl;
                    exit(EXIT_FAILURE);
                }
                else
                {
                    mem.mem_stack.push(mem.registers[index]);
                    mem.reg_f[index] = false;
                }
            }
            else
            {
                mem.registers[index] = mem.mem_stack.top();
                mem.reg_f[index] = true;
                mem.mem_stack.pop();
            }
        }
    };

    template <class T>
    struct OPERATION : command<T>
    {
        void run(memory<T> &mem, T &arg) override
        {
            mem.registers[static_cast<int>(RegisterName::OP)] = mem.mem_stack.top();
            mem.mem_stack.pop();
            if (arg == static_cast<T>(CommandName::ADD))
                mem.registers[static_cast<int>(RegisterName::RX)] = mem.registers[static_cast<int>(RegisterName::OP)] + mem.mem_stack.top();
            else if (arg == static_cast<T>(CommandName::SUB))
                mem.registers[static_cast<int>(RegisterName::RX)] = mem.registers[static_cast<int>(RegisterName::OP)] - mem.mem_stack.top();
            else if (arg == static_cast<T>(CommandName::MUL))
                mem.registers[static_cast<int>(RegisterName::RX)] = mem.registers[static_cast<int>(RegisterName::OP)] * mem.mem_stack.top();
            else if (arg == static_cast<T>(CommandName::DIV))
                mem.registers[static_cast<int>(RegisterName::RX)] = mem.registers[static_cast<int>(RegisterName::OP)] / mem.mem_stack.top();

            mem.mem_stack.push(mem.registers[static_cast<int>(RegisterName::OP)]);
            mem.mem_stack.push(mem.registers[static_cast<int>(RegisterName::RX)]);
        }
    };

    template <class T>
    struct OUT_IN : command<T>
    {
        void run(memory<T> &mem, T &arg) override
        {
            if (arg == static_cast<T>(CommandName::OUT))
            {
                std::cout << mem.mem_stack.top() << std::endl;
                mem.mem_stack.pop();
            }
            else
            {
                T in;
                std::cin >> in;
                mem.mem_stack.push(in);
            }
        }
    };

    template <class T>
    struct JMP : command<T>
    {
        void run(memory<T> &mem, T &arg, T &label, int &PC) override
        {
            if (arg == static_cast<T>(CommandName::JMP))
            {
                PC = static_cast<int>(label);
            }
            else if (arg == static_cast<T>(CommandName::JEQ))
            {
                mem.registers[static_cast<int>(RegisterName::OP)] = mem.mem_stack.top();
                mem.mem_stack.pop();
                if (mem.registers[static_cast<int>(RegisterName::OP)] == mem.mem_stack.top())
                    PC = static_cast<int>(label);
                else
                    PC++;
                mem.mem_stack.push(mem.registers[static_cast<int>(RegisterName::OP)]);
            }
            else if (arg == static_cast<T>(CommandName::JNE))
            {
                mem.registers[static_cast<int>(RegisterName::OP)] = mem.mem_stack.top();
                mem.mem_stack.pop();
                if (mem.registers[static_cast<int>(RegisterName::OP)] != mem.mem_stack.top())
                    PC = static_cast<int>(label);
                else
                    PC++;
                mem.mem_stack.push(mem.registers[static_cast<int>(RegisterName::OP)]);
            }
            else if (arg == static_cast<T>(CommandName::JA))
            {
                mem.registers[static_cast<int>(RegisterName::OP)] = mem.mem_stack.top();
                mem.mem_stack.pop();
                if (mem.registers[static_cast<int>(RegisterName::OP)] > mem.mem_stack.top())
                    PC = static_cast<int>(label);
                else
                    PC++;
                mem.mem_stack.push(mem.registers[static_cast<int>(RegisterName::OP)]);
            }
            else if (arg == static_cast<T>(CommandName::JAE))
            {
                mem.registers[static_cast<int>(RegisterName::OP)] = mem.mem_stack.top();
                mem.mem_stack.pop();
                if (mem.registers[static_cast<int>(RegisterName::OP)] >= mem.mem_stack.top())
                    PC = static_cast<int>(label);
                else
                    PC++;
                mem.mem_stack.push(mem.registers[static_cast<int>(RegisterName::OP)]);
            }
            else if (arg == static_cast<T>(CommandName::JB))
            {
                mem.registers[static_cast<int>(RegisterName::OP)] = mem.mem_stack.top();
                mem.mem_stack.pop();
                if (mem.registers[static_cast<int>(RegisterName::OP)] < mem.mem_stack.top())
                    PC = static_cast<int>(label);
                else
                    PC++;
                mem.mem_stack.push(mem.registers[static_cast<int>(RegisterName::OP)]);
            }
            else if (arg == static_cast<T>(CommandName::JBE))
            {
                mem.registers[static_cast<int>(RegisterName::OP)] = mem.mem_stack.top();
                mem.mem_stack.pop();
                if (mem.registers[static_cast<int>(RegisterName::OP)] <= mem.mem_stack.top())
                    PC = static_cast<int>(label);
                else
                    PC++;
                mem.mem_stack.push(mem.registers[static_cast<int>(RegisterName::OP)]);
            }
            else if (arg == static_cast<T>(CommandName::CALL))
            {
                mem.function.push(PC + 1);
                PC = static_cast<int>(label);
                mem.registers[static_cast<int>(RegisterName::BP)] = mem.mem_stack.get_len();
            }
        }
    };

    template <class T>
    struct RET : command<T>
    {
        void run(memory<T> &mem, int &PC) override
        {
            PC = mem.function.top();
            mem.function.pop();
            if (mem.mem_stack.get_len() > mem.registers[static_cast<int>(RegisterName::BP)])
                mem.mem_stack.set_len(mem.registers[static_cast<int>(RegisterName::BP)]);
        }
    };

    template <class T>
    class cpu : parser
    {
    public:
        void launch_parser()
        {
            parse();
            std::cout << "Parsed successfully" << std::endl;
        }

        void start()
        {
            int end = PC;
            PC = 0;
            while (program[PC][0] != static_cast<size_t>(CommandName::BEGIN)) // Begin find
            {
                if (program[PC][0] == static_cast<size_t>(CommandName::END) || PC == end)
                {
                    std::cerr << "Error: Program must have a BEGIN command" << std::endl;
                    exit(EXIT_FAILURE);
                }
                PC++;
            }
            PC++;

            while (program[PC][0] != static_cast<size_t>(CommandName::END))
            {
                if (PC == end)
                {
                    std::cerr << "Error: Program must have an END command" << std::endl;
                    exit(EXIT_FAILURE);
                }
                if (program[PC][0] == static_cast<size_t>(CommandName::PUSH))
                {
                    PUSH<T> command;
                    command.run(mem, program[PC][1]);
                    PC++;
                }
                else if (program[PC][0] == static_cast<size_t>(CommandName::POP))
                {
                    POP<T> command;
                    command.run(mem);
                    PC++;
                }
                else if (program[PC][0] == static_cast<size_t>(CommandName::PUSHR) || program[PC][0] == static_cast<size_t>(CommandName::POPR))
                {
                    PUSHR_POPR<T> command;
                    command.run(mem, program[PC][0], program[PC][1]);
                    PC++;
                }
                else if (program[PC][0] == static_cast<size_t>(CommandName::ADD) || program[PC][0] == static_cast<size_t>(CommandName::SUB) ||
                         program[PC][0] == static_cast<size_t>(CommandName::MUL) || program[PC][0] == static_cast<size_t>(CommandName::DIV))
                {
                    OPERATION<T> command;
                    command.run(mem, program[PC][0]);
                    PC++;
                }
                else if (program[PC][0] == static_cast<size_t>(CommandName::OUT) || program[PC][0] == static_cast<size_t>(CommandName::IN))
                {
                    OUT_IN<T> command;
                    command.run(mem, program[PC][0]);
                    PC++;
                }
                else if (program[PC][0] == static_cast<size_t>(CommandName::JMP) || program[PC][0] == static_cast<size_t>(CommandName::JEQ) ||
                         program[PC][0] == static_cast<size_t>(CommandName::JNE) || program[PC][0] == static_cast<size_t>(CommandName::JA) ||
                         program[PC][0] == static_cast<size_t>(CommandName::JAE) || program[PC][0] == static_cast<size_t>(CommandName::JB) ||
                         program[PC][0] == static_cast<size_t>(CommandName::JBE) || program[PC][0] == static_cast<size_t>(CommandName::CALL))
                {
                    JMP<T> command;
                    command.run(mem, program[PC][0], program[PC][1], PC);
                }
                else if (program[PC][0] == static_cast<size_t>(CommandName::RET))
                {
                    RET<T> command;
                    command.run(mem, PC);
                }
                else
                {
                    PC++;
                }
            }
        }

    private:
        memory<T> mem;
    };
}
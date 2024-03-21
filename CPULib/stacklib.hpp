#pragma once

namespace CPULib
{
    template <class T>
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
        void push(T &&value)
        {
            push(value);
        }

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
            {
                std::cerr << "Error: Can not pop, stack is empty" << std::endl;
            }
        }

        bool empty()
        {
            return len == 0;
        }

        T &top()
        {
            if (len != 0)
            {
                return arr[len - 1];
            }
            else
            {
                std::cerr << "Error: Can not get top value, stack is empty" << std::endl;
            }
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
}
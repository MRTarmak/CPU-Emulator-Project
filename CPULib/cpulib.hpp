#pragma once

#include <iostream>

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
        }

        // Copy assignment operator
        stack<T> &operator=(const stack<T> &other)
        {
            len = other.len;
            delete[] arr;
            arr = new T[len];
            for (int i = 0; i < len; i++)
            {
                arr[i] = other.arr[i];
            }

            return *this;
        }

        // Copy constructors
        stack(const stack<T> &other)
        {
            len = other.len;
            arr = new T[len];
            for (int i = 0; i < len; i++)
            {
                arr[i] = other.arr[i];
            }
        }

        explicit stack(const T &other)
        {
            len = 1;
            arr = new T[1];
            arr[0] = other;
        }

        // Move assignment operator
        stack<T> &operator=(stack<T> &&other) noexcept
        {
            len = other.len;
            delete[] arr;
            arr = other.arr;
            other.arr = nullptr;
            other.len = 0;

            return *this;
        }

        // Move constructors
        stack(stack<T> &&other) noexcept
        {
            len = other.len;
            arr = other.arr;
            other.arr = nullptr;
            other.len = 0;
        }

        explicit stack(T &&other)
        {
            len = 1;
            arr = new T[1];
            arr[0] = std::move(other);
        }

        // Class methods
        void push(T value)
        {
            arr[len] = value;
            len++;
            arr = (T*)realloc(arr, sizeof(T) * len);
        }

        void pop()
        {
            len--;
            if (len != 0)
                arr = (T*)realloc(arr, sizeof(T) * len);
        }

        bool empty()
        {
            return len == 0;
        }

        T top()
        {
            return arr[len - 1];
        }

    private:
        T *arr = nullptr;
        int len = 0;
    };
}
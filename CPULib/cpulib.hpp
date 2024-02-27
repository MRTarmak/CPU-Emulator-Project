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
            arr = std::copy(other.arr);

            return *this;
        }

        // Copy constructor
        stack(const stack<T> &other)
        {
            this = other;
        }

        // Move assignment operator
        stack<T> &operator=(stack<T> &&other) noexcept
        {
            len = other.len;
            arr = std::move(other.arr);

            return *this;
        }

        // Move constructor
        stack(stack<T> &&other) noexcept
        {
            this = other;
        }

        void push(T value)
        {
            arr[len] = value;
            len++;
            arr = (T*)realloc(arr, sizeof(T) * len);
        }

        void pop()
        {
            len--;
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
        T *arr;
        int len;
    };
}
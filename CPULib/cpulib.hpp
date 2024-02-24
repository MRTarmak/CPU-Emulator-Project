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

        void push(T value)
        {
            arr[len] = value;
            len++;
            arr = (T)realloc(arr, sizeof(T) * len);
        }

        void pop()
        {
            len--;
            arr = (T)realloc(arr, sizeof(T) * len);
        }

        bool empty()
        {
            return len == 0 ? 1 : 0;
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
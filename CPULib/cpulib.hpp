#pragma once

#include <iostream>

namespace CPULib
{
    template<class T>
    class stack
    {
    public:
        // Initialization constructor
        stack()
        {
            arr = (T)malloc(sizeof(T));
            len = 0;
        }

        // Copy constructor
        stack(const T &num)
        {
            arr[len] = std::copy(num);
            len++;
            arr = (T) realloc(arr, sizeof(T) * len);
        }

    private:
        T *arr;
        int len;
    };
}

#include "String.hpp"

#include <math.h>
#include <stdlib.h>

namespace kon
{
    String::String(const char *str)
    {
        *this = str;
    }

    String::~String()
    {
        delete[] m_buffer;
    }

    void String::ResizeBuffer(i32 size)
    {
        if(m_buffer != nullptr) delete[] m_buffer;
        m_buffer = new char[size+1];
    }

    i32 String::Hash()
    {
        constexpr int prime = 31;
        i32 h = 0;
        for(int c = 0; c < m_size; c++)
            h += m_buffer[c] * pow(prime, c);
        return h;
    }

    i32 String::Hash(const char *string)
    {
        constexpr int prime = 31;
        i32 h = 0;
        int size = strlen(string);
        for(int c = 0; c < size; c++)
            h += string[c] * pow(prime, c);
        return h;
    }
}

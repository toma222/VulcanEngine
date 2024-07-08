
#include "String.hpp"
#include "kon/core/Logging.hpp"

#include <math.h>
#include <stdlib.h>

namespace kon
{
    String::String()
    {
        *this = "kn_null";
    }

    String::String(const char *str)
    {
        *this = str;
    }

    String::String(const String &string)
    {
        *this = string.c_str();
    }

    String::~String()
    {
		if(m_buffer != nullptr) delete[] m_buffer;
    }

    void String::ResizeBuffer(u32 size)
    {
        if(m_buffer != nullptr) delete[] m_buffer;
        m_buffer = new char[size+1];
    }

    u32 String::Hash() const
    {
        constexpr int prime = 31;
        u32 h = 0;
        for(u32 c = 1; c < m_size; c++)
            h += (m_buffer[c] * (int)pow(31, c));
        return h;
    }

    u32 String::Hash(const char *string)
    {
        constexpr int prime = 31;
        i32 h = 0;
        int size = strlen(string);
        for(int c = 0; c < size; c++)
            h += string[c] * pow(prime, c);
        return h;
    }
}

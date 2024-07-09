
#include "String.hpp"
#include "kon/core/Logging.hpp"

#include <cstring>
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
		m_size = size+1;
    }

    u32 String::Hash() const
    {
        constexpr int prime = 31;
        u32 h = 0;
        for(u32 c = 1; c < m_size; c++)
            h += m_buffer[c] * (int)pow(c, 3);
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

	String String::SubString(int index, int len) const
	{
		char s[len];
		memcpy(&s, m_buffer+index, len);
		s[len] = '\0';
		return String(s);
	}

	String String::SubString(int len) const
	{
		return SubString(0, len);
	}

	int String::IndexOfString(String str) const
	{
		return (int)(strstr(m_buffer, str.c_str()) - m_buffer);
	}

	int String::IndexOfChar(char c) const
	{
		return (int)(strchr(m_buffer, c) - m_buffer);
	}

	int String::LastIndexOfChar(char c) const
	{
		return (int)(strrchr(m_buffer, c) - m_buffer);
	}

	String String::AppendString(const String &s) const
	{
		char str[s.m_size + m_size];
		strcpy(str, m_buffer);
		strcat(str, s.c_str());
		return String(str);
	}
}

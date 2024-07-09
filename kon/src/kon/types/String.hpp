
#pragma once

#include "../core/Core.hpp"

#include <cstring>

namespace kon
{
    struct knAPI String
    {
    public:
        String();
        String(const char *str);
        String(const String &string);
        ~String();

        const char *c_str() const {return m_buffer; }

		String SubString(int index, int len) const;
		String SubString(int len) const;
		int IndexOfString(String str) const;
		int IndexOfChar(char c) const;
		String AppendString(const String &s) const;
		int LastIndexOfChar(char c) const;

        // from c_string
        String &operator =(const char *str)
        {
            m_size = strlen(str);
            ResizeBuffer(m_size);
            strcpy(m_buffer, str);
            return *this;
        }

        String &operator =(const String &s)
        {
            *this = (s.c_str());
            return *this;
        }

        char operator [](int i) const
        {
            return m_buffer[i];
        }

        u32 GetSize() { return m_size; }

        u32 Hash() const;
        static u32 Hash(const char *string);

        // size+1 for the null terminating byte
        void ResizeBuffer(u32 size);

        bool operator ==(const String &string) const
        {
            return (strcmp(string.m_buffer, m_buffer) == 0);
        }

    private:
        char *m_buffer {nullptr};
        u32 m_size {0};
    };
}

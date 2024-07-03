
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

        // from c_string
        String &operator =(const char *str)
        {
            m_size = strlen(str);
            ResizeBuffer(m_size);
            strcpy(m_buffer, str);
            // m_buffer[m_size+1] = '\0';
            return *this;
        }

        char operator [](int i) const
        {
            return m_buffer[i];
        }

        i32 GetSize() { return m_size; }

        i32 Hash() const;
        static i32 Hash(const char *string);

        // size+1 for the null terminating byte
        void ResizeBuffer(i32 size);

        bool operator ==(const String &string) const
        {
            return (strcmp(string.m_buffer, m_buffer) == 0);
        }

    private:
        char *m_buffer {nullptr};
        i32 m_size {0};
    };
}

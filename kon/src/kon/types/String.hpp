
#pragma once

#include "../core/Core.hpp"

#include <cstring>

namespace kon
{
    struct knAPI String
    {
    public:
        String(const char *str);
        ~String();

        const char *c_str() const {return m_buffer; }

        // from c_string
        String &operator =(const char *str)
        {
            m_size = strlen(str);
            ResizeBuffer(m_size);
            strcpy(m_buffer, str);
            m_buffer[m_size+1] = '\0';
            return *this;
        }

        char operator [](int i) const
        {
            return m_buffer[i];
        }

        i32 GetSize() { return m_size; }

        i32 Hash();
        static i32 Hash(const char *string);

        void ResizeBuffer(i32 size);

    private:
        char *m_buffer {nullptr};
        i32 m_size {0};
    };
}

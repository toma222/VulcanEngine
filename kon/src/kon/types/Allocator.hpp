
#pragma once

#include "kon/core/Core.hpp"

namespace kon
{
    struct Page
    {
        u32 size;
        u8 *memory;

        Page *nextPage;
        Page *firstPage;
    };

    template<typename T>
    class Allocator
    {
    public: 
        PageAllocator(u8 minPageSize);
        virtual ~PageAllocator();

        u8 *allocate(u8 size) override;
        virtual void free(u8 *memory, u8 size) override;

    private:
        Page *m_page;
    };
}

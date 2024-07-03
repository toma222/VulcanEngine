
#pragma once

#include "kon/core/Core.hpp"

namespace kon
{
    // this class is not fully implemented yet

    template<typename T>
    class Allocator
    {
    public:
        struct Page
        {
        public:
            T *pageStart;
            T *pageIndex;
            
            u32 pageSize;

            Page *next {nullptr};

        public:
            ~Page()
            {
                delete[] pageStart;
                delete next;
            }
        };

    public: 
        Allocator(u32 minPageSize);
        ~Allocator();

        T *Allocate() override;
        void Free(u8 *memory, u8 size);

    private:
        void AllocateNewPage(u8 *pageSize);

    private:
        Page *m_firstPage;
        Page *m_page;
        u32 m_minPageSize;
    };
}

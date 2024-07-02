
#include "Allocator.hpp"

#include <cmath>
#include <kon/core/Logging.hpp>

namespace kon
{
    #define LOG2(X) ((unsigned) (8*sizeof (unsigned long long) - __builtin_clzll((X)) - 1))

    template<typename T>
    Allocator<T>::Allocator(u32 minPageSize)
    {
        m_page = new Page();
        m_firstPage = m_page;

        u32 pageSize = pow(ceil(LOG2(sizeof(T))), 2);
        KN_TRACE("page allocator with page size %u", pageSize);
        m_minPageSize = pageSize;
        m_page.pageSize = sizeof(T);
    }

    template<typename T>
    Allocator<T>::~Allocator()
    {
        delete m_page;
    }

    template<typename T>
    T *Allocator<T>::Allocate()
    {

    }

    template<typename T>
    void Allocator<T>::AllocateNewPage(u8 *pageSize)
    {

    }

    template<typename T>
    void Allocator<T>::Free(u8 *memory, u8 size)
    {

    }
}

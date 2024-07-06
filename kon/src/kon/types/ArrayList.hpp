
#pragma once

#include "kon/core/Core.hpp"
#include "kon/core/Logging.hpp"

// #include <string>

namespace kon
{
    // A resize-able array
    // just like java
    template<typename T>
    class ArrayList
    {
    public:
        ArrayList()
        {
            m_index = 0;
            m_capacity = 1;
            m_array = new T[1];
        }

        ArrayList(u32 elements)
        {
            m_index = 0;
            m_capacity = elements;
            m_array = new T[elements];
        }

        ArrayList(const ArrayList<T> &list)
        {
            KN_TRACE("ArrayList copy constructor");
        }
        
        ~ArrayList()
        {
            delete[] m_array;
        }

        // copies the element
        void Add(T element)
        {
            // Adds to the top of the array
            if(m_index >= m_capacity)
            {
                Reserve(++m_capacity);
            }

            m_array[m_index] = element;
            m_index++;
        }

        // FIXME: ub lmao
        /*
        T Get(int index) const
        {
            // if(index <= m_capacity && index >= 0)
            return m_array[index];
            // else
            //     return m_array[0]; // just return the first value
        }
        */

        T &Get(u32 index) const
        {
            return m_array[index];
        }

        T GetCopy(u32 index) const
        {
            return m_array[index];
        }

        void Set(u32 index, T element)
        {
            if(m_capacity >= index && index < 0) return; // outside bounds
            m_array[index] = element;
        }

        void Pop()
        {
            if(m_index != 0) m_index--;
        }
        
        int Index() const { return m_index; };
        int Capacity() const { return m_capacity; }
        T *GetData() const { return m_array; }

        void Reserve(u32 elements)
        {
            m_capacity = elements;
            T *swap = new T[m_capacity];
            for(u32 i = 0; i < m_index; i++)
                swap[i] = m_array[i];
            delete[] m_array;
            m_array = swap;
        }

    private:
        T *m_array;
        u32 m_index;
        u32 m_capacity;
    };
}

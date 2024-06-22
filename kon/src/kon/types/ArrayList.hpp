
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

        ArrayList(const ArrayList<T> &list)
        {
            KN_TRACE("ArrayList copy constructor");
        }
        
        ~ArrayList()
        {
            delete[] m_array;
        }

        void Add(T element)
        {
            // Adds to the top of the array
            if(m_index >= m_capacity) Resize();
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

        T &Get(int index) const
        {
            return m_array[index];
        }

        void Set(int index, T element)
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

        // std::string ToString();

    private:
        void Resize()
        {
            // copy the array into an array twice it's size
            KN_TRACE("Resize");
            m_capacity *= 2;
            T *swap = new T[m_capacity];
            for(int i = 0; i < m_index; i++)
                swap[i] = m_array[i];
            delete[] m_array;
            m_array = swap;
        }
    
    private:
        T *m_array;
        int m_index;
        int m_capacity;
    };
}


#pragma once

#include "kon/core/Core.hpp"
#include "kon/core/Logging.hpp"
#include <cstring>

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

		void Erase(u32 i, u32 size=1)
		{
			MoveRange(i, i + size, m_capacity - i - size);
		}

		void MoveRange(u32 dest, u32 source, u32 count)
		{
			if(count) memmove(m_array+dest, m_array+source, count * sizeof(T));
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

		T &operator[](int i) { return m_array[i]; }

    private:
        T *m_array;
        u32 m_index;
        u32 m_capacity;
    };
}

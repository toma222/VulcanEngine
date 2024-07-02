
#pragma once

#include <kon/core/Logging.hpp>

namespace kon
{
    template<typename T>
    struct UniquePointer
    {
    public:
        UniquePointer(T *ref)
            : m_ref(ref) {}

        UniquePointer()
            : m_ref(nullptr) {}

        ~UniquePointer()
        {
            if(m_ref == nullptr) KN_WARN("UniquePointer freeing nullptr");
            delete m_ref;
        }

        UniquePointer(const UniquePointer<T> &) = delete;

    public:
        // this DOES NOT FREE THE CURRENT POINTER
        void Set(T *ref) { m_ref = ref; };
        T *Get() { return m_ref; }

    public:
        operator bool() const { return (m_ref == nullptr); }
        T *operator->() const { return m_ref; }
        
    private:
        T *m_ref { nullptr };
    };
}


#pragma once

#include <kon/core/Logging.hpp>
#include <kon/core/Object.hpp>
#include <kon/core/RefCount.hpp>

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

    // Must inherit from the object class
    // NOT TESTED
    template<typename T>
    class SharedPointer
    {
    public:
        SharedPointer(T *ref)
            : m_ref(ref) {}

        SharedPointer(const SharedPointer<T> &sp)
        {
            m_ref = sp.m_ref;
            m_ref->AddRef();
        }

        ~SharedPointer()
        {
            // check if this is the last ref
            if(m_ref->GetRefs() == 1)
            {
                delete m_ref;
            }else{
                m_ref->SubtractRef();
            }
        }

        T *Get() const { return m_ref; }

    public:
        operator bool() const { return (m_ref == nullptr); }
        T *operator->() const { return m_ref; }

    private:
        T *m_ref {nullptr};
    };
}

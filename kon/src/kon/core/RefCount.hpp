
#pragma once

#include <kon/core/Core.hpp>

namespace kon
{
    template<typename T>
    class SharedPointer;

    class RefCount
    {
    public:
        RefCount();
        virtual ~RefCount();

    // make shared pointer friends
    // i dont want other classes look at this crap
    public:
        void AddRef();
        void SubtractRef();

        u32 GetRefs() const;
        bool LastRef() const { return (m_refs == 1); }

    private:
        u32 m_refs {1}; // starts with the object who owns the ref
    };
}

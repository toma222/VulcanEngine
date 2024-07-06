
#include "RefCount.hpp"

namespace kon
{
    RefCount::RefCount()
        : m_refs(1) {}

    RefCount::~RefCount() = default;

    void RefCount::AddRef()
    {
        m_refs++;
    }

    void RefCount::SubtractRef()
    {
        m_refs--;
    }

    u32 RefCount::GetRefs() const
    {
        return m_refs;
    }
}


#include "Object.hpp"

namespace kon
{
    bool Object::IsInstanceOf(u32 hash)
    {
        return (GetTypeInfo()->m_hash == hash);
    }

    bool Object::IsInstanceOf(const TypeInfo *info)
    {
        return IsInstanceOf(info->m_hash);
    }
}


#include "Map.hpp"

#include <kon/types/String.hpp>

namespace kon
{
    template<>
    u32 Hash(int &hash)
    {
        return hash;
    }

    template<>
    u32 Hash(String &hash)
    {
        return hash.Hash();
    }
}

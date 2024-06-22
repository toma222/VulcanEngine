
#pragma once

#include "kon/types/String.hpp"
#include "kon/core/Core.hpp"

namespace kon
{
    class knAPI Variant
    {
        union
        {
            float f;
            int i;
            String s;
        };
    };
}

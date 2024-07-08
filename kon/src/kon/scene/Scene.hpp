
#pragma once

#include <kon/core/Core.hpp>

namespace kon
{
    class Component
    {
    public:
        Component() = default;
        virtual ~Component() = default;

        // virtual u32 GetString() { return String::Hash(""); }
    };
}

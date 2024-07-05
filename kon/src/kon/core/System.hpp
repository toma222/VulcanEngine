
#pragma once

#include <kon/core/Object.hpp>

namespace kon
{
    class System : public Object
    {
    KN_OBJECT(System, Object)

    public:
        System() = default;
        virtual ~System() = default;

        virtual void Update() = 0;
    };
}

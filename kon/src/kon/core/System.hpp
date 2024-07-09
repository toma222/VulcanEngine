
#pragma once

#include "kon/resource/Resource.hpp"
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
		// virtual void RegisterResources(LoadResourceArray &lra) const = 0;
    };
}

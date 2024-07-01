
#pragma once

#include "kon/core/Object.hpp"
#include "kon/types/String.hpp"

namespace kon
{
    class Resource : public Object
    {
    KN_OBJECT(Resource, Object)

    public:
        

    public:
        Resource();
        virtual ~Resource() = 0;

        bool Valid();

        void LoadResource(const String &path);
    };
}

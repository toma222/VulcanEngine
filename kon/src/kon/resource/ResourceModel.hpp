
#pragma once

#include "Resource.hpp"

#include <tiny_obj_loader.h>
#include <glm/glm.hpp>

namespace kon
{
    class ResourceModel : public Resource
    {
    KN_OBJECT(ResourceModel, Object)

    public:
        struct ModelVertex
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoord;
            glm::vec3 color;
        };

        struct Shape
        {

        };

    public:
        ResourceModel();
        ~ResourceModel();
    };
}

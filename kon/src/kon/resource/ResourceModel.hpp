
#pragma once

#include "Resource.hpp"

#include <tiny_obj_loader.h>
#include <glm/glm.hpp>

#include <kon/types/ArrayList.hpp>
#include <kon/types/Pointer.hpp>

namespace kon
{
    class ResourceModel : public Resource
    {
    KN_OBJECT(ResourceModel, Object)

    public:
        struct ModelVertex
        {
            glm::vec3 position;
			glm::vec3 color;
            glm::vec2 texCoord;
			glm::vec3 normal;
        };

        struct Material
        {

        };

        struct Shape
        {
            ArrayList<ModelVertex> verticies;
            ArrayList<int> indicies;
            Material material;
        };

        struct Model
        {
            ArrayList<Shape> shapes;
        };

    public:
        bool Valid() override;
        void LoadResource(const String &path) override;

    public:
        ResourceModel();
        ~ResourceModel();

        const Shape *GetShape() { return m_shape.Get(); }

    private:
        // UniquePointer<Model> m_model;
        UniquePointer<Shape> m_shape; // for now the whole thing is defined as one big ol shape
    };
}

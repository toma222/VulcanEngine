
#include "ResourceModel.hpp"

namespace kon
{
    ResourceModel::ResourceModel()
        : m_shape() // defaults to nullptr
    {}

    ResourceModel::~ResourceModel() = default;

    bool ResourceModel::Valid()
    {
        return (m_shape);
    }

    void ResourceModel::LoadResource(const String &path)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, path.c_str()))
        {
            KN_WARN("tinyobj loader error: %s\n warn %s\n", warn, err);
        }

        KN_TRACE("setting shape");
        m_shape.Set(new Shape());

        for(const auto &shape : shapes)
        {
            m_shape->verticies.Reserve(shape.mesh.indices.size());
            m_shape->indicies.Reserve(shape.mesh.indices.size());

            for (const auto &index : shape.mesh.indices)
            {
                // KN_TRACE("new vertex");
                ModelVertex vertex {};

                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                // we need to flip the texture coordnates
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};

                vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				};

                m_shape->verticies.Add(vertex);
                m_shape->indicies.Add(m_shape->indicies.Index());
            }
        }
    }
}

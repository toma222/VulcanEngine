#pragma once

#include <kon/resource/Resource.hpp>
#include <kon/types/ArrayList.hpp>

namespace kon
{
    class ResourceBytecode : public Resource
    {
	KN_OBJECT(ResourceBytecode, Resource)

    public:
        ResourceBytecode();
        ~ResourceBytecode() override;

    public:
        bool Valid() override;
        void LoadResource(const String &path) override;

    public:
        ArrayList<char> m_data {};
    };
}

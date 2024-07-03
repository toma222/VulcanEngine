
#pragma once

#include <kon/resource/Resource.hpp>
#include <kon/types/ArrayList.hpp>

namespace kon
{
    class ResourceRawfile : public Resource
    {
    public:
        ResourceRawfile();
        ~ResourceRawfile();

    public:
        bool Valid() override;
        void LoadResource(const String &path) override;

    public:
        ArrayList<char> m_data {};
    };
}

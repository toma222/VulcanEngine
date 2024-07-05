
#pragma once

#include <kon/resource/Resource.hpp>
#include <kon/types/ArrayList.hpp>

namespace kon
{
    class ResourceRawfile : public Resource
    {
    public:
        ResourceRawfile();
        virtual ~ResourceRawfile();

    public:
        bool Valid() override;
        void LoadResource(const String &path) override;
        char *GetData() { return m_data; }
        size_t GetSize() { return m_size; }

    public:
        char *m_data {nullptr};
        size_t m_size {0};
    };
}

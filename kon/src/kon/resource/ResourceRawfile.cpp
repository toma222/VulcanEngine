
#include "ResourceRawfile.hpp"

namespace kon
{
    ResourceRawfile::ResourceRawfile()
    {

    }

    ResourceRawfile::~ResourceRawfile()
    {

    }

    bool ResourceRawfile::Valid()
    {
        return (m_data.Index() != 0);
    }

    void ResourceRawfile::LoadResource(const String &path)
    {
        FILE *file;
        file = fopen(path.c_str(), "r");
        if(file == nullptr) KN_WARN("could not read file %s", path.c_str());
        fseek(file, 0L, SEEK_END);
        u32 fileSize = ftell(file);
        fseek(file, 0L, SEEK_SET);

        m_data.Reserve(fileSize);

        fgets(m_data.GetData(), 100, file);
        fclose(file);
    }
}


#include "ResourceRawfile.hpp"

#include <fstream>

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
        return (m_data != nullptr);
    }

    void ResourceRawfile::LoadResource(const String &path)
    {
        /*
        FILE *file;
        file = fopen(path.c_str(), "r");
        if(file == nullptr) KN_WARN("could not read file %s", path.c_str());
        fseek(file, 0L, SEEK_END);
        m_size = ftell(file);
        fseek(file, 0L, SEEK_SET);

        fgets(m_data, m_size, file);

        fclose(file);
        */

        std::ifstream file(path.c_str(), std::ios::ate | std::ios::binary);

        if(!file.is_open()) KN_WARN("could not read file %s", path.c_str());

        size_t fileSize = (size_t) file.tellg();
        // std::vector<char> buffer(fileSize);
        file.seekg(0);
        m_data = new char[fileSize];
        file.read(m_data, fileSize);
        file.close();

        m_size = fileSize;
    }
}

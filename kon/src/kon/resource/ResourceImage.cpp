
#include "ResourceImage.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

#include <kon/core/Logging.hpp>

namespace kon
{
    ResourceImage::ResourceImage() = default;
    
    ResourceImage::~ResourceImage()
    {
		KN_WARN("here");
        stbi_image_free(m_imageData);
    }

    bool ResourceImage::Valid()
    {
        return m_valid;
    }

    void ResourceImage::LoadResource(const String &path)
    {
        m_loadPath = path.c_str();
        m_stringHash = path.Hash();

        int requestedChannels = STBI_rgb_alpha;
        m_imageData = stbi_load(path.c_str(), &m_width, &m_height, (int*)(&m_channels), STBI_rgb_alpha);
        if (!m_imageData)
        {
            KN_ERROR("Image resource failed to load image: path -> \n%s", path.c_str());
            return; // dont let it set valid to true
        }

        m_valid = true;
    }
}

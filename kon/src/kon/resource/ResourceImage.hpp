
#pragma once

#include "Resource.hpp"

namespace kon
{
    class ResourceImage : public Resource
    {
    KN_OBJECT(ResourceImage, Resource)

    public:
        enum ImageChannels : int
        {
            ImageChannels_Mono = 1,
            ImageChannels_RGB = 3,
            ImageChannels_RGBA = 4
        };

    public:
        ResourceImage();
        ~ResourceImage();

    public:
        bool Valid() override;

        // for now the image will always be loaded rgba
        void LoadResource(const String &path) override;

    public:
        int GetWidth() { return m_width; }
        int GetHeight() { return m_height; }
        ImageChannels GetChannels() { return m_channels; }

    private:
        bool m_valid {false};
        u8 *m_imageData;

        int m_width {};
        int m_height {};
        ImageChannels m_channels {};
    };
}

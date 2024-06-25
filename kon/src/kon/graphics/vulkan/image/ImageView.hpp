
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <kon/graphics/vulkan/Device.hpp>

namespace kon
{
    class ImageView
    {
    public:
        ImageView(Device *device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels=1);
        ImageView() = default;
		~ImageView();

        VkImageView Get() { return m_view; }

		void Destroy();

    private:
        VkImageView m_view { VK_NULL_HANDLE };
		Device *m_device;
    };
}


#pragma once

#include "kon/graphics/vulkan/commands/CommandPool.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <kon/graphics/vulkan/Device.hpp>

namespace kon
{
    /*
        TECHNICALY this image is a 2d image
    */



    class Image
    {
    public:
        Image() = default;
        Image(Device *device, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
               VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
        ~Image();

        VkImage Get() { return m_image; }

        uint32_t GetWidth() { return m_width; }
        uint32_t GetHeight() { return m_height; }

		void Destroy();

    public:
        static VkFormat FindSupportedFormat(Device *device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        void TransitionImageLayout(CommandPool *pool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

    private:
        VkImage m_image { VK_NULL_HANDLE };
        VkDeviceMemory m_imageMemory { VK_NULL_HANDLE };
        VkDevice m_device { VK_NULL_HANDLE };
		VkImageLayout m_layout;

        uint32_t m_width;
        uint32_t m_height;
    };
}

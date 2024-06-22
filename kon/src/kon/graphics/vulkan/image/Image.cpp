
#include "Image.hpp"

#include <kon/debug/Debug.hpp>
#include <kon/core/Logging.hpp>

namespace kon
{
    VkFormat Image::FindSupportedFormat(Device *device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
    {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(device->GetPhysicalDevice(), format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        KN_WARN("could not find supported format");
    }

    void Image::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
    {

    }

    uint32_t findMemoryType(Device *device, uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        KN_INSTRUMENT_FUNCTION()

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(device->GetPhysicalDevice(), &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if (typeFilter & (1 << i)) {
                return i;
            }
        }

        KN_ERROR("failed to find suitable memory type!");

        return 0;
    }

    Image::Image(Device *device, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
               VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
    {
        KN_INSTRUMENT_FUNCTION()

        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = numSamples;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(device->Get(), &imageInfo, nullptr, &m_image) != VK_SUCCESS) {
           KN_WARN("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device->Get(), m_image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(device, memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device->Get(), &allocInfo, nullptr, &m_imageMemory) != VK_SUCCESS) {
            KN_WARN("failed to allocate image memory!");
        }

        vkBindImageMemory(device->Get(), m_image, m_imageMemory, 0);
    }

    Image::~Image()
    {
        KN_INSTRUMENT_FUNCTION()
        
        vkDestroyImage(m_device, m_image, nullptr);
        vkFreeMemory(m_device, m_imageMemory, nullptr);
    }
}

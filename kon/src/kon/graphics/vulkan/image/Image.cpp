
#include "Image.hpp"
#include "kon/graphics/vulkan/commands/CommandBuffer.hpp"
#include "kon/graphics/vulkan/commands/CommandPool.hpp"
#include "vulkan/vulkan_core.h"

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

    void Image::TransitionImageLayout(CommandPool *pool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
    {
		CommandBuffer commandBuffer = pool->StartSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = m_image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = mipLevels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = 0; // TODO
        barrier.dstAccessMask = 0; // TODO

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        } else {
            KN_ERROR("unsupported layout transition!");
        }

        if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

            if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT)
			{
                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        } else {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        }

        vkCmdPipelineBarrier(
            commandBuffer.Get(),
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );
		
		commandBuffer.EndSingleTimeCommands();
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

	void Image::Destroy()
	{
		KN_INSTRUMENT_FUNCTION()
        
		if(m_image == VK_NULL_HANDLE) return;
        vkDestroyImage(m_device, m_image, nullptr);
        vkFreeMemory(m_device, m_imageMemory, nullptr);
	}

    Image::~Image()
    {
		Destroy();
    }
}

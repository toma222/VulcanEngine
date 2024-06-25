
#include "ImageView.hpp"
#include "vulkan/vulkan_core.h"

#include <kon/debug/Debug.hpp>

namespace kon
{
    ImageView::ImageView(Device *device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
    	: m_device(device)
	{
        KN_INSTRUMENT_FUNCTION()
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        viewInfo.subresourceRange.aspectMask = aspectFlags;

        // VkImageView imageView;
        if (vkCreateImageView(device->Get(), &viewInfo, nullptr, &m_view) != VK_SUCCESS) {
            KN_WARN("failed to create image view!");
        }
    }

	void ImageView::Destroy()
	{
		vkDestroyImageView(m_device->Get(), m_view, nullptr);
	}

	ImageView::~ImageView()
	{
		if(m_view == VK_NULL_HANDLE) return;
		Destroy();
	}
}

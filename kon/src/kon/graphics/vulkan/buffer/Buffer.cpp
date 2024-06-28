
#include "Buffer.hpp"
#include "kon/debug/Debug.hpp"
#include "kon/graphics/vulkan/commands/CommandBuffer.hpp"
#include "kon/graphics/vulkan/commands/CommandPool.hpp"
#include "vulkan/vulkan_core.h"

namespace kon
{
	uint32_t Buffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_device->GetPhysicalDevice(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
        		return i;
    		}
		}

		KN_ERROR("can not find suitable memory type");
		return 0;
	}

	Buffer::Buffer(Device *device, CommandPool *pool, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
		: m_device(device), m_pool(pool)
	{
		KN_INSTRUMENT_FUNCTION()
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_device->Get(), &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_device->Get(), m_buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(m_device->Get(), &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(m_device->Get(), m_buffer, m_memory, 0);
	}

	//Buffer::Buffer(Device *device, CommandPool *pool)
	//{

	//}:

	void Buffer::BindData(void *data, size_t size)
	{
		KN_INSTRUMENT_FUNCTION();
		void* dst;
    	vkMapMemory(m_device->Get(), m_memory, 0, size, 0, &dst);
        	memcpy(dst, data, size);
    	vkUnmapMemory(m_device->Get(), m_memory);
	}

	void Buffer::Copy(Buffer *dst, VkDeviceSize size)
	{
		// Buffer *to = new Buffer(m_pool);
		KN_INSTRUMENT_FUNCTION()

		CommandBuffer commandBuffer = m_pool->StartSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer.Get(), m_buffer, dst->Get(), 1, &copyRegion);

        commandBuffer.EndSingleTimeCommands();

		// return to;
	}

	void Buffer::Copy(Image *dst, uint32_t width, uint32_t height)
	{
		CommandBuffer commandBuffer = m_pool->StartSingleTimeCommands();

		VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

		// this would go crazy for something like sprite maps
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(
            commandBuffer.Get(),
            m_buffer,
            dst->Get(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );


		commandBuffer.EndSingleTimeCommands();
	}

	Buffer::~Buffer()
	{
		vkDestroyBuffer(m_device->Get(), m_buffer, nullptr);
		vkFreeMemory(m_device->Get(), m_memory, nullptr);
	}
}


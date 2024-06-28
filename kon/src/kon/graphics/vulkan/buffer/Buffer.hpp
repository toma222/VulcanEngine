
#pragma once

#include "kon/graphics/vulkan/commands/CommandPool.hpp"
#include "kon/graphics/vulkan/image/Image.hpp"
#include "vulkan/vulkan_core.h"

namespace kon
{
	class Buffer
	{
	public:
		Buffer(Device *device, CommandPool *pool, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		// Buffer(Device *device, CommandPool *pool);
		~Buffer();

		void BindData(void *data, size_t size);

		void Copy(Buffer *dst, VkDeviceSize size);
		void Copy(Image *dst, uint32_t width, uint32_t height);

		VkBuffer Get() { return m_buffer; }
		VkDeviceMemory GetMemory() { return m_memory; }

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);


	public:
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;

		Device *m_device;
		CommandPool *m_pool;
	};
}


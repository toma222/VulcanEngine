
#include "UniformBuffer.hpp"
#include "kon/graphics/vulkan/buffer/Buffer.hpp"
#include "kon/graphics/vulkan/commands/CommandPool.hpp"
#include <cstring>

namespace kon
{
	UniformBuffer::UniformBuffer(Device *device, CommandPool *pool, size_t size)
		: m_device(device), m_size(size)
	{
		m_buffer = new Buffer(device, pool, size, 
						 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		vkMapMemory(m_device->Get(), m_buffer->GetMemory(), 0, size, 0, &m_uniformBuffer);
	}

	void UniformBuffer::CopyDataToBuffer(void *data, size_t size)
	{
		memcpy(m_uniformBuffer, data, size);
	}


	UniformBuffer::~UniformBuffer()
	{
		delete m_buffer;
	}
}


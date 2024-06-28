
#pragma once

#include "kon/graphics/vulkan/buffer/Buffer.hpp"
namespace kon
{
	class IndexBuffer
	{
	public:
		IndexBuffer(Device *device, CommandPool *pool, void *data, size_t size);
		~IndexBuffer();

		Buffer *Get() { return m_buffer; }

	private:
		Device *m_device;
		CommandPool *m_commandPool;
		Buffer *m_buffer;
	};
}


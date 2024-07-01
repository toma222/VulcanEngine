
#pragma once

#include "kon/graphics/vulkan/Device.hpp"
#include "kon/graphics/vulkan/buffer/Buffer.hpp"
#include "kon/graphics/vulkan/commands/CommandPool.hpp"
#include "vulkan/vulkan_core.h"

namespace kon
{
	class UniformBuffer
	{
	public:
		UniformBuffer(Device *device, CommandPool *pool, size_t size);
		~UniformBuffer();

		Buffer *Get() { return m_buffer; }
		size_t GetSize() { return m_size; }

		void CopyDataToBuffer(void *data, size_t size);

		bool Valid() { return (m_buffer->Get() != VK_NULL_HANDLE); }

	private:
		Device *m_device;
		Buffer *m_buffer;
		size_t m_size;

		void *m_uniformBuffer;
	};
}


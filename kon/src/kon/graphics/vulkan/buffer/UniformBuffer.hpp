
#pragma once

#include "kon/graphics/vulkan/Device.hpp"
#include "kon/graphics/vulkan/buffer/Buffer.hpp"
#include "kon/graphics/vulkan/commands/CommandPool.hpp"

namespace kon
{
	class UniformBuffer
	{
	public:
		UniformBuffer(Device *device, CommandPool *pool, size_t size);
		~UniformBuffer();

		Buffer *Get() { return m_buffer; }

		void CopyDataToBuffer(void *data, size_t size);

	private:
		Device *m_device;
		Buffer *m_buffer;

		void *m_uniformBuffer;
	};
}


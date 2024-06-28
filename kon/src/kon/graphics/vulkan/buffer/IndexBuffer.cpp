
#include "IndexBuffer.hpp"

namespace kon
{
	IndexBuffer::IndexBuffer(Device *device, CommandPool *pool, void *data, size_t size)
	{
		Buffer *stagingBuffer = new Buffer(device, pool, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	
		stagingBuffer->BindData(data, size);

		m_buffer = new Buffer(device, pool, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		stagingBuffer->Copy(m_buffer, size);

		delete stagingBuffer;
	}
	
	IndexBuffer::~IndexBuffer()
	{
		delete m_buffer;
	}
}


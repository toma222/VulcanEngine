#include "VertexBuffer.hpp"
#include "kon/core/Logging.hpp"
#include "kon/debug/Debug.hpp"
#include "vulkan/vulkan_core.h"

namespace kon
{
	constexpr VkFormat GetFormatFromShaderType(const ShaderType type)
	{
		switch (type)
		{
			case(ShaderType::Float4): return VK_FORMAT_R32G32B32A32_SFLOAT;
			case(ShaderType::Float3): return VK_FORMAT_R32G32B32_SFLOAT;
			case(ShaderType::Float2): return VK_FORMAT_R32G32_SFLOAT;

			default: return VK_FORMAT_R32_UINT;
		}
	}

	VertexDescription::VertexDescription() = default;

	VertexDescription::VertexDescription(size_t size, int descriptions)
		: m_size(size)
	{
		m_attributeDescriptions.resize(descriptions);

		m_binding.binding = 0;
        m_binding.stride = size;
        m_binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	}

	VertexDescription::~VertexDescription() = default;

	void VertexDescription::Add(const ShaderType type, size_t offset)
	{
		m_attributeDescriptions[m_index].binding = 0;
        m_attributeDescriptions[m_index].location = m_index;
        m_attributeDescriptions[m_index].format = GetFormatFromShaderType(type);
        m_attributeDescriptions[m_index].offset = offset;
		m_index++;
	}

	VertexBuffer::VertexBuffer(Device *device, CommandPool *pool, void *data, size_t size)
		: m_device(device), m_commandPool(pool)
	{
		KN_INSTRUMENT_FUNCTION()

		Buffer *stagingBuffer = new Buffer(m_device, m_commandPool, size,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		stagingBuffer->BindData(data, size);

		m_vertexBuffer = new Buffer(device, pool, size,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		stagingBuffer->Copy(m_vertexBuffer, size);
		
		delete stagingBuffer;
	}

	VertexBuffer::~VertexBuffer()
	{
		delete m_vertexBuffer;
	}

	void VertexBuffer::SetDescription(VertexDescription description)
	{	
		m_description = description;
	}
}


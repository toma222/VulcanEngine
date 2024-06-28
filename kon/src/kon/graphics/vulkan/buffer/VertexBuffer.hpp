
#pragma once

#include "kon/graphics/vulkan/buffer/Buffer.hpp"
#include "kon/graphics/vulkan/commands/CommandPool.hpp"
#include "vulkan/vulkan_core.h"
#include <kon/graphics/vulkan/Instance.hpp>
#include <kon/graphics/vulkan/Device.hpp>
#include <kon/graphics/Window.hpp>
#include <kon/graphics/vulkan/image/ImageView.hpp>
#include <kon/graphics/vulkan/image/Image.hpp>
#include <kon/graphics/vulkan/image/Framebuffer.hpp>
#include <vector>

namespace kon
{
	enum class ShaderType
	{
		Float2,
		Float3,
		Float4,
	};

	// this should just get copied into the vertexbuffer struct
	struct VertexDescription
	{
	public:
		VertexDescription();
		VertexDescription(size_t size, int descriptions);
		VertexDescription(const VertexDescription&) = default;
		~VertexDescription();

		void Add(const ShaderType type, size_t offset);

		std::vector<VkVertexInputAttributeDescription> GetDescription() { return m_attributeDescriptions; }
		VkVertexInputBindingDescription GetBinding() { return m_binding; }

	private:
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions {};
		VkVertexInputBindingDescription m_binding {};
		int m_index {0};
		size_t m_size;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(Device *device, CommandPool *pool, void *data, size_t size);
		~VertexBuffer();

		void SetDescription(VertexDescription description);

		Buffer *Get() { return m_vertexBuffer; }
		VertexDescription &GetDescription() { return m_description; }

	public:
		Buffer *m_vertexBuffer;
		
		Device *m_device;
		CommandPool *m_commandPool;

		VertexDescription m_description;
	};
}


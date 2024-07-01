
#pragma once

#include "kon/core/Core.hpp"
#include "kon/graphics/vulkan/Swapchain.hpp"
#include "kon/graphics/vulkan/buffer/IndexBuffer.hpp"
#include "kon/graphics/vulkan/buffer/VertexBuffer.hpp"
#include "kon/graphics/vulkan/descriptor/DescriptorSets.hpp"
#include "kon/graphics/vulkan/pipeline/RenderPass.hpp"
#include "kon/graphics/vulkan/pipeline/ShaderModule.hpp"
#include "vulkan/vulkan_core.h"

#include <vector>
#include <vulkan/vulkan.h>

namespace kon
{

	// right now only supports one push constant
	struct PushConstantDef
	{
	public:
		VkShaderStageFlags stageFlags;
		uint32_t offset;
		uint32_t size;

	public:
		VkPushConstantRange Get() const
		{
			return VkPushConstantRange{stageFlags, offset, size};
		}
	};

	struct RenderPipelineLayoutDef
	{
		std::vector<PushConstantDef> pushConstants;
		DescriptorSets *descriptorSets;
		DescriptorLayout *descriptorLayout;

		ShaderModule *vertexShader;
		ShaderModule *fragmentShader;

		VertexBuffer *vertexBuffer;
		IndexBuffer *indexBuffer;

		RenderPass *renderPass;

		// u32 pushConstantCount;
		// VkPushConstantRange pushConstant;
	};

	class RenderPipeline
	{
	public:
		RenderPipeline(Device *device, Swapchain *swapchain, CommandPool *commandPool, const RenderPipelineLayoutDef &def);
		~RenderPipeline();

		VkPipeline Get() { return m_pipeline; }
		VkPipelineLayout GetLayout() { return m_pipelineLayout; }

	private:
		VkPipeline m_pipeline;
		VkPipelineLayout m_pipelineLayout;

		Device *m_device;
		CommandPool *m_commandPool;

		VertexBuffer *m_vertexBuffer;
		IndexBuffer *m_indexBuffer;
	};
}



#pragma once

// #include "vulkan/vulkan_core.h"

#include "kon/core/Logging.hpp"
#include "kon/graphics/vulkan/buffer/UniformBuffer.hpp"
#include "kon/graphics/vulkan/image/ImageView.hpp"
#include "kon/graphics/vulkan/image/TextureSampler.hpp"
#include "vulkan/vulkan_core.h"

namespace kon
{
	enum class DescriptorType
	{
		Uniform,
		TextureSampler
	};

	constexpr VkDescriptorType DescriptorTypeToVulkanType(const DescriptorType type)
	{
		switch (type)
		{
			case kon::DescriptorType::Uniform : return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			case kon::DescriptorType::TextureSampler : return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		}
	}

	class Descriptor
	{
	public:
		// Descriptor() = delete;
		virtual ~Descriptor() = default;

		virtual DescriptorType GetType() = 0;
		virtual VkDescriptorSetLayoutBinding GetDescriptorBinding(int binding) = 0;
		virtual VkWriteDescriptorSet GetWriteSet(VkDescriptorSet set, int binding) = 0;
	};

	class DescriptorUniform : public Descriptor
	{
	public:
		DescriptorUniform(UniformBuffer *ub, int count=1)
			: m_uniformBuffer(ub)
		{
			m_info = new VkDescriptorBufferInfo();
            m_info->buffer = m_uniformBuffer->Get()->Get();
            m_info->offset = 0; // error is here
            m_info->range = m_uniformBuffer->GetSize();
		}

		~DescriptorUniform() override
		{
			delete m_info;
		}

 		DescriptorType GetType() override { return DescriptorType::Uniform; }

		VkDescriptorSetLayoutBinding GetDescriptorBinding(int binding) override
		{
			VkDescriptorSetLayoutBinding uboLayoutBinding{};
			uboLayoutBinding.binding = binding;
        	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        	uboLayoutBinding.descriptorCount = 1;
        	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
			return uboLayoutBinding;
		}

		VkWriteDescriptorSet GetWriteSet(VkDescriptorSet set, int binding) override
		{
			VkWriteDescriptorSet write {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.dstSet = set;
        	write.dstBinding = binding;
            write.dstArrayElement = 0;
            write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            write.descriptorCount = 1;
            write.pBufferInfo = m_info;

			return write;
		}


	public:
		UniformBuffer *m_uniformBuffer;
		VkDescriptorBufferInfo *m_info;
	};

	class DescriptorTextureSampler : public Descriptor
	{
	public:
		DescriptorTextureSampler(TextureSampler *ts, ImageView *view, int count=1)
			: m_textureSampler(ts), m_imageView(view)
		{
			// VkDescriptorImageInfo imageInfo{};
			m_info = new VkDescriptorImageInfo();
            m_info->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            m_info->imageView = m_imageView->Get();
            m_info->sampler = m_textureSampler->Get();
		}

		~DescriptorTextureSampler() override
		{
			delete m_info;
		}

		DescriptorType GetType() override { return DescriptorType::TextureSampler; }

		VkDescriptorSetLayoutBinding GetDescriptorBinding(int binding) override
		{
			VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        	samplerLayoutBinding.binding = binding;
        	samplerLayoutBinding.descriptorCount = 1;
        	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        	samplerLayoutBinding.pImmutableSamplers = nullptr;
			return samplerLayoutBinding;
		}

		VkWriteDescriptorSet GetWriteSet(VkDescriptorSet set, int binding) override
		{
			
			VkWriteDescriptorSet write {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.dstSet = set;
        	write.dstBinding = binding;
            write.dstArrayElement = 0;
            write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            write.descriptorCount = 1;
            write.pImageInfo = m_info;

			return write;
		}

	public:
		TextureSampler *m_textureSampler;
		ImageView *m_imageView;
		VkDescriptorImageInfo *m_info;
	};
}


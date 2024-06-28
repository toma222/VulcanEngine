
#pragma once

#include "kon/graphics/vulkan/Device.hpp"
#include "kon/graphics/vulkan/descriptor/DescriptorPool.hpp"
#include "vulkan/vulkan_core.h"
#include <initializer_list>

namespace kon
{
	enum class DescriptorType
	{
		UniformBuffer,
		CombinedImageSampler
	};

	struct DescriptorWrite
	{
	public:
		union DescriptorData
		{
			VkDescriptorImageInfo m_imageInfo;
			VkDescriptorBufferInfo m_bufferInfo;
		};

	public:
		DescriptorWrite(DescriptorType type, DescriptorData data);
		~DescriptorWrite();

	};

	struct DescriptorSet
	{
		DescriptorSet(std::initializer_list<DescriptorWrite> writes);			

		std::vector<DescriptorWrite> m_writes;
	};

	class DescriptorSets
	{
	public:
		DescriptorSets(Device *device, DescriptorPool *descriptorPool, std::vector<DescriptorSet> &sets, int amount);
		~DescriptorSets();

		std::vector<VkDescriptorSet> &Get() { return m_descriptorSets; }

	private:
		std::vector<VkDescriptorSet> m_descriptorSets;
		Device *m_device;
	};
}


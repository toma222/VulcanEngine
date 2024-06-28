
#pragma once

#include "kon/graphics/vulkan/Device.hpp"
#include "vulkan/vulkan_core.h"
#include <vector>

namespace kon
{
	struct DescriptonSizeFactory
	{
		void Add(VkDescriptorType type, uint32_t amount);

		std::vector<VkDescriptorPoolSize> m_poolSizes {};
		int m_size {0};
		int m_maxAmount {0};
	};

	class DescriptorPool
	{
	public:
		DescriptorPool(Device *device, DescriptonSizeFactory &factory);
		~DescriptorPool();

		VkDescriptorPool Get() { return m_descriptorPool; }

	private:
		Device *m_device;

		VkDescriptorPool m_descriptorPool;
	};
}


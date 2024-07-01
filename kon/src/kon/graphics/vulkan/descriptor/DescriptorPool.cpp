
#include "DescriptorPool.hpp"
#include "kon/graphics/vulkan/descriptor/Descriptor.hpp"
#include "vulkan/vulkan_core.h"

namespace kon
{
	void DescriptonSizeFactory::Add(DescriptorType type, uint32_t amount)
	{
		m_poolSizes.push_back(VkDescriptorPoolSize{});
		m_poolSizes[m_size].type = DescriptorTypeToVulkanType(type);
        m_poolSizes[m_size].descriptorCount = amount;
		m_size++;
		if(amount > m_maxAmount) m_maxAmount = amount;
	}

	DescriptorPool::DescriptorPool(Device *device, DescriptonSizeFactory &factory)
		: m_device(device)
	{
		VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(factory.m_size);
        poolInfo.pPoolSizes = factory.m_poolSizes.data();
        poolInfo.maxSets = factory.m_maxAmount;
        
        if (vkCreateDescriptorPool(m_device->Get(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
		{
            KN_WARN("failed to create descriptor pool!");
        }

	}

	DescriptorPool::~DescriptorPool()
	{
		vkDestroyDescriptorPool(m_device->Get(), m_descriptorPool, nullptr);
	}
}


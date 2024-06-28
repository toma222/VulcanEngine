
#include "DescriptorSets.hpp"
#include "kon/debug/Debug.hpp"
#include "kon/graphics/vulkan/descriptor/DescriptorPool.hpp"

namespace kon
{
	DescriptorWrite::DescriptorWrite(DescriptorType type, DescriptorData data)
	{

	}

	DescriptorWrite::~DescriptorWrite()
	{

	}

	DescriptorSet::DescriptorSet(std::initializer_list<DescriptorWrite> writes)
		: m_writes(writes) {}

	DescriptorSets::DescriptorSets(Device *device, DescriptorPool *descriptorPool, std::vector<DescriptorSet> &sets, int amount)
		: m_device(device)
	{
		KN_INSTRUMENT_FUNCTION()
        std::vector<VkDescriptorSetLayout> layouts(amount, m_descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool->Get();
        allocInfo.descriptorSetCount = static_cast<uint32_t>(amount);
        allocInfo.pSetLayouts = layouts.data();

        m_descriptorSets.resize(amount);
        if (vkAllocateDescriptorSets(m_device->Get(), &allocInfo, m_descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

	}

	DescriptorSets::~DescriptorSets()
	{

	}
}


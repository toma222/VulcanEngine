
#include "DescriptorSets.hpp"
#include "kon/debug/Debug.hpp"
#include "kon/graphics/vulkan/descriptor/DescriptorPool.hpp"
#include "vulkan/vulkan_core.h"

namespace kon
{
	DescriptorVector::DescriptorVector() = default;

	DescriptorVector::~DescriptorVector()
	{
		for (Descriptor *d : m_descriptors)
		{
			delete d;
		}
	}

	DescriptorLayout::DescriptorLayout(Device *device, std::vector<DescriptorVector*> descriptors)
		: m_device(device), m_descriptionVector(descriptors)
	{
		KN_INSTRUMENT_FUNCTION()

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(descriptors[0]->m_bindings.size());
        layoutInfo.pBindings = descriptors[0]->m_bindings.data();

        if (vkCreateDescriptorSetLayout(m_device->Get(), &layoutInfo, nullptr, &m_layout) != VK_SUCCESS)
		{
            KN_WARN("failed to create descriptor set layout!");
        }
	}

	DescriptorLayout::~DescriptorLayout()
	{
		for (DescriptorVector *v : m_descriptionVector)
		{
			delete v;
		}

		vkDestroyDescriptorSetLayout(m_device->Get(), m_layout, nullptr);
	}

	DescriptorSets::DescriptorSets(Device *device, DescriptorPool *descriptorPool, DescriptorLayout *layout, int count)
		: m_device(device)
	{
		KN_INSTRUMENT_FUNCTION()
        std::vector<VkDescriptorSetLayout> layouts(count, layout->Get());
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool->Get();
        allocInfo.descriptorSetCount = static_cast<uint32_t>(count);
        allocInfo.pSetLayouts = layouts.data();

        m_descriptorSets.resize(count);
        if (vkAllocateDescriptorSets(m_device->Get(), &allocInfo, m_descriptorSets.data()) != VK_SUCCESS) {
            KN_ERROR("failed to allocate descriptor sets!");
        }

        // for (size_t i = 0; i < count; i++)
		int d = 0;
		for(DescriptorVector *descVector : layout->m_descriptionVector)
		{
			std::vector<VkWriteDescriptorSet> descriptorWrites {};
			for(int i = 0; i < descVector->m_descriptors.size(); i++)
			{
				descriptorWrites.push_back(
						descVector->m_descriptors[i]->GetWriteSet(m_descriptorSets[d], i));
			}

            vkUpdateDescriptorSets(m_device->Get(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        	d++;
		}
	}

	DescriptorSets::~DescriptorSets()
	{
		
	} 
}


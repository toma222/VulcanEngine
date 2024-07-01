
#pragma once

#include "kon/graphics/vulkan/Device.hpp"
#include "kon/graphics/vulkan/descriptor/DescriptorPool.hpp"
#include "vulkan/vulkan_core.h"
#include <initializer_list>
#include <utility>

namespace kon
{
	class DescriptorSets;

	struct DescriptorVector
	{
	public:
		DescriptorVector();
		~DescriptorVector();

		template<typename T, typename ...Args>
		void Add(Args&& ...args)
		{
			//m_descriptors.push_back(
			Descriptor *t = new T(std::forward<Args>(args)...);
			m_descriptors.push_back(t);
			m_bindings.push_back(t->GetDescriptorBinding(m_bindings.size()));
		}

	public:
		std::vector<Descriptor*> m_descriptors {};
		std::vector<VkDescriptorSetLayoutBinding> m_bindings {};
	};

	struct DescriptorLayout
	{
	public:
		DescriptorLayout(Device *device, std::vector<DescriptorVector*> descriptors);
		~DescriptorLayout();

		VkDescriptorSetLayout Get() { return m_layout; }

	private:
		Device *m_device;
		std::vector<DescriptorVector*> m_descriptionVector;
		VkDescriptorSetLayout m_layout;

		friend DescriptorSets;
	};

	class DescriptorSets
	{
	public:
		DescriptorSets(Device *device, DescriptorPool *descriptorPool, DescriptorLayout *layout, int count);
		~DescriptorSets();

		std::vector<VkDescriptorSet> &Get() { return m_descriptorSets; }

	private:
		std::vector<VkDescriptorSet> m_descriptorSets;
		Device *m_device;
	};
}


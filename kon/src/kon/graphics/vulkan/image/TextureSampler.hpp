
#pragma once

#include "kon/graphics/vulkan/Device.hpp"
#include "vulkan/vulkan_core.h"
namespace kon
{
	class TextureSampler
	{
	public:
		TextureSampler(Device *device);
		~TextureSampler();

		VkSampler Get() { return m_sampler; }

	private:
		Device *m_device;
		VkSampler m_sampler;
	};
}



#pragma once

#include "kon/graphics/vulkan/Device.hpp"
#include "vulkan/vulkan_core.h"

namespace kon
{
	class ShaderModule
	{
	public:
		ShaderModule(Device *device, std::vector<char> &code);
		~ShaderModule();

		VkShaderModule Get() { return m_module; }
	
	private:
		VkShaderModule m_module;
		Device *m_device;
	};
}


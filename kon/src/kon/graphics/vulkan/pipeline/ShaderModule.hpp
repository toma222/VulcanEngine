
#pragma once

#include "kon/graphics/vulkan/Device.hpp"
#include "vulkan/vulkan_core.h"
#include <kon/types/ArrayList.hpp>

namespace kon
{
	class ShaderModule
	{
	public:
		ShaderModule(Device *device, char *code, size_t size);
		~ShaderModule();

		VkShaderModule Get() { return m_module; }
	
	private:
		VkShaderModule m_module;
		Device *m_device;
	};
}


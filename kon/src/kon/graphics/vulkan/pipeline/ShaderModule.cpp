
#include "ShaderModule.hpp"
#include "kon/debug/Debug.hpp"
#include "vulkan/vulkan_core.h"

namespace kon
{
	ShaderModule::ShaderModule(Device *device, std::vector<char> &code)
		: m_device(device)
	{
		KN_INSTRUMENT_FUNCTION()
        VkShaderModuleCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if(vkCreateShaderModule(m_device->Get(), &createInfo, nullptr, &m_module) != VK_SUCCESS)
        {
        	KN_WARN("failed to create shader module");
        }
	}
	
	ShaderModule::~ShaderModule()
	{
		vkDestroyShaderModule(m_device->Get(), m_module, nullptr);
	}
}



#include "kon/graphics/vulkan/image/TextureSampler.hpp"
#include "kon/debug/Debug.hpp"
#include "kon/graphics/vulkan/image/TextureImage.hpp"
#include "vulkan/vulkan_core.h"

namespace kon
{
	TextureSampler::TextureSampler(Device *device)
		: m_device(device)
	{
		KN_INSTRUMENT_FUNCTION()
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(m_device->GetPhysicalDevice(), &properties);
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = VK_LOD_CLAMP_NONE;

        if (vkCreateSampler(m_device->Get(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS)
		{
            KN_WARN("failed to create texture sampler!");
        }

	}

	TextureSampler::~TextureSampler()
	{
		vkDestroySampler(m_device->Get(), m_sampler, nullptr);
	}
}


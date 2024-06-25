
#pragma once

#include "vulkan/vulkan_core.h"
#include <kon/graphics/vulkan/commands/CommandBuffer.hpp>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <kon/graphics/vulkan/Device.hpp>

namespace kon
{
	// we do not have support for multiple buffer :D
    class CommandPool
    {
    public:
        CommandPool(Device *device, int i);
        ~CommandPool();

		VkCommandPool Get() { return m_commandPool; }
		std::vector<VkCommandBuffer> &GetBuffers() { return m_buffers; }
		VkCommandBuffer GetBuffer(int i) { return m_buffers[i]; }
		// VkCommandPool *Get() { return *m }

		CommandBuffer StartSingleTimeCommands();

    private:
        VkCommandPool m_commandPool;
		std::vector<VkCommandBuffer> m_buffers;
		// CommandBuffer m_primaryBuffer;
		// VkCommandBuffer m_primaryBuffer;
		Device *m_device;
    };
}

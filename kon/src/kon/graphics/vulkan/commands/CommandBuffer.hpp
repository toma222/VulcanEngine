
#pragma once

#include "kon/graphics/vulkan/Device.hpp"
#include "vulkan/vulkan_core.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kon
{
	class CommandPool;

    class CommandBuffer
    {
    public:
        CommandBuffer(Device *device, CommandPool *commandPool);
		CommandBuffer(Device *device, CommandPool *commandPool, VkCommandBuffer buffer);

        ~CommandBuffer();

        // void StartSingleTimeCommands();
        void EndSingleTimeCommands();

		VkCommandBuffer Get() { return m_buffer; };

    private:
        VkCommandBuffer m_buffer { VK_NULL_HANDLE };
		Device *m_device;
		CommandPool *m_commandPool;
    };
}

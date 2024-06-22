
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <kon/graphics/vulkan/Device.hpp>

namespace kon
{
    class CommandPool
    {
    public:
        CommandPool(Device *device);
        ~CommandPool();

    private:
        VkCommandPool m_commandPool;
    };
}

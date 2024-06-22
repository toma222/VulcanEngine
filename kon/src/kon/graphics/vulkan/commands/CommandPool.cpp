
#include "CommandPool.hpp"

namespace kon
{
    CommandPool::CommandPool(Device *device)
    {
        QueueFamilyIndices queueFamilyIndices = device->FindQueueFamilies();

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(device->Get(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
            KN_WARN("failed to create command pool!");
        }
    }

    CommandPool::~CommandPool()
    {

    }
}

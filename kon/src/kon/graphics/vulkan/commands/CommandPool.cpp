
#include "CommandPool.hpp"
#include "kon/graphics/vulkan/commands/CommandBuffer.hpp"

namespace kon
{
    CommandPool::CommandPool(Device *device, int i)
		: m_device(device) // , m_primaryBuffer(device, this)
    {
        QueueFamilyIndices queueFamilyIndices = device->FindQueueFamilies();

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(device->Get(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
            KN_WARN("failed to create command pool!");
        }
		
		m_buffers.resize(i);
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = i;

		// VkCommandBuffer buffer = m_primaryBuffer.Get();
	
		if (vkAllocateCommandBuffers(device->Get(), &allocInfo, m_buffers.data()) != VK_SUCCESS) {
    		KN_WARN("failed to allocate command buffers!");
		}
    }

	CommandBuffer CommandPool::StartSingleTimeCommands()
	{
		VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(m_device->Get(), &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return CommandBuffer(m_device, this, commandBuffer);

	}

    CommandPool::~CommandPool()
    {
		vkDestroyCommandPool(m_device->Get(), m_commandPool, nullptr);
    }
}

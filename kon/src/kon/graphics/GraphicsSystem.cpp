
#pragma once

#include "GraphicsSystem.hpp"

namespace kon
{
    constexpr int FRAMES_IN_FLIGHT = 1;

    GraphicsSystem::GraphicsSystem(Context *context, GLFWwindow *window)
    {
        m_vulkanInstance = new Instance(window);
        m_device = new Device(m_vulkanInstance);
        m_commandPool = new CommandPool(m_device, FRAMES_IN_FLIGHT);
        m_swapchain = new Swapchain(m_vulkanInstance, m_device, m_commandPool, window);

        PipelineShaded *shadedPipeline = new PipelineShaded(context, m_device, m_swapchain, m_commandPool);
        m_pipeline = static_cast<Pipeline*>(shadedPipeline);

        /*
            Create sync objects
        */
        m_imageAvailableSemaphores.resize(FRAMES_IN_FLIGHT);
        m_renderFinishedSemaphores.resize(FRAMES_IN_FLIGHT);
        m_inFlightFences.resize(FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(m_device->Get(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_device->Get(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(m_device->Get(), &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {
                assert("failed to create semaphores!");
            }
        }
    }

    GraphicsSystem::~GraphicsSystem()
    {
        delete m_vulkanInstance;
        delete m_device;
        delete m_commandPool;
        delete m_swapchain;
        delete m_pipeline;
    }

    void GraphicsSystem::Update()
    {
        vkWaitForFences(m_device->Get(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
        
        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(m_device->Get(), m_swapchain->Get(), UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            m_swapchain->RecreateSwapchain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        
        vkResetFences(m_device->Get(), 1, &m_inFlightFences[m_currentFrame]);

        vkResetCommandBuffer(m_commandPool->GetBuffer(m_currentFrame), 0);
        RenderFrameData d;
        d.swapchainImage = imageIndex;
        d.renderExtent = m_swapchain->GetSwapchainExtent();
        m_pipeline->GetDrawCommands(m_commandPool->GetBuffer(m_currentFrame), d);
        // RecordCommandBuffer(m_commandPool->GetBuffer(m_currentFrame), imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphores[m_currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
		VkCommandBuffer b = m_commandPool->GetBuffer(m_currentFrame);
		submitInfo.pCommandBuffers = &b;

        VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphores[m_currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(m_device->GetGraphicsQueue(), 1, &submitInfo, m_inFlightFences[m_currentFrame]) != VK_SUCCESS) {
            assert("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapChains[] = {m_swapchain->Get()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        VkResult presentResult = vkQueuePresentKHR(m_device->GetPresentQueue(), &presentInfo);
        if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR)
        {
            // m_swapchain->RecreateSwapchain();
        } else if (presentResult != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }
}

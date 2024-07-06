
#pragma once

#include "kon/graphics/vulkan/commands/CommandPool.hpp"
#include "kon/graphics/vulkan/pipeline/RenderPass.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include <kon/graphics/vulkan/Instance.hpp>
#include <kon/graphics/vulkan/Device.hpp>
#include <kon/graphics/Window.hpp>
#include <kon/graphics/vulkan/image/ImageView.hpp>
#include <kon/graphics/vulkan/image/Image.hpp>
#include <kon/graphics/vulkan/image/Framebuffer.hpp>

namespace kon
{
    class Swapchain
    {
    public:
        Swapchain(Instance *instance, Device *device, CommandPool *commandPool, GLFWwindow *window);
        ~Swapchain();

   		void CreateFramebuffers();
		void RecreateSwapchain();
		VkSwapchainKHR Get() { return m_swapChain; }

		VkExtent2D GetSwapchainExtent() { return m_swapChainExtent; }
		VkFormat GetSwapchainFormat() { return m_swapChainImageFormat; }
		// Framebuffer *GetFramebuffer(int i) { return m_swapChainFramebuffers[i]; }

        std::vector<ImageView*> &GetSwapchainImageViews() { return m_swapChainImageViews; }

    private:
        void DestroySwapchain();
        
        void CreateSwapchain();
        void GetSwapchainImages();
        void CreateImageViews();

        // void CreateColorResources();
        // void CreateDepthResources();

        

    private:
        VkSwapchainKHR m_swapChain;
        std::vector<VkImage> m_swapChainImages;
        std::vector<ImageView*> m_swapChainImageViews;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;

        Instance *m_instance;
        Device *m_device;
		CommandPool *m_commandPool;
		RenderPass *m_renderPass {nullptr};
        GLFWwindow *m_window;
    };
}

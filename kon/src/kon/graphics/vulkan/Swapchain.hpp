
#pragma once

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
        Swapchain(Instance *instance, Device *device, const Window *window);
        ~Swapchain();

        // void RecreateSwapchain(Instance *instance, Device *device, const Window *windows);
    
    private:
        void DestroySwapchain();
        void RecreateSwapchain();

        void CreateSwapchain();
        void GetSwapchainImages();
        void CreateImageViews();
        void CreateFramebuffers();

        void CreateColorResources();
        void CreateDepthResources();

    private:
        VkSwapchainKHR m_swapChain;
        std::vector<VkImage> m_swapChainImages;
        std::vector<ImageView> m_swapChainImageViews;
        std::vector<Framebuffer> m_swapChainFramebuffers;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;

        Image m_colorImage {};
        ImageView m_colorImageView {};
        Image m_depthImage {};
        ImageView m_depthImageView {};

        Instance *m_instance;
        Device *m_device;
        const Window *m_window;
    };
}

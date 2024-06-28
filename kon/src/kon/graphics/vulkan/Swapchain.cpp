
#include "Swapchain.hpp"

#include <kon/debug/Debug.hpp>
#include "Device.hpp"
#include "kon/core/Logging.hpp"
#include "kon/graphics/vulkan/commands/CommandPool.hpp"
#include "kon/graphics/vulkan/pipeline/RenderPass.hpp"
#include "vulkan/vulkan_core.h"
#include <algorithm>
#include <kon/graphics/vulkan/image/ImageView.hpp>
#include <array>

namespace kon
{
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
    {
        for (const auto &availableFormat : availableFormats)
        {
            if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
               availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
    {
        for(const auto &availablePresentMode : availablePresentModes)
        {
            if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode; // prefer mailbox (cuz it fast and no tearing)
            }

			if(availablePresentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
			{
				return availablePresentMode;
			}
        }

        // :3
		KN_WARN("using present mode FIFO_KHR");
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window)
    {
        if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        } else {
            int width, height;

            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    Swapchain::Swapchain(Instance *instance, Device *device, CommandPool *commandPool, GLFWwindow *window)
        : m_instance(instance), m_device(device), m_commandPool(commandPool), m_window(window)
    {
        KN_INSTRUMENT_FUNCTION()
        CreateSwapchain();

        GetSwapchainImages();
        CreateImageViews();		
		CreateColorResources();
		CreateDepthResources();
    }

    Swapchain::~Swapchain()
    {
		DestroySwapchain();
    }

	void Swapchain::RecreateSwapchain()
	{
		KN_INSTRUMENT_FUNCTION()

		DestroySwapchain();

		CreateSwapchain();
        GetSwapchainImages();
        CreateImageViews();
		CreateColorResources();
		CreateDepthResources();
		CreateFramebuffers();
	}

    void Swapchain::CreateSwapchain()
    {
        KN_INSTRUMENT_FUNCTION()

        SwapChainSupportDetails swapChainSupport = m_device->QuerySwapChainSupport();

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes); // VK_PRESENT_MODE_IMMEDIATE_KHR; // 
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, m_window);
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1; // :3

        if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        /*
            It is also possible that you’ll render images to a separate image first to perform operations like post-processing.
            In that case you may use a value like VK_IMAGE_USAGE_TRANSFER_DST_BIT instead and use a memory operation
            to transfer the rendered image to a swap chain image.
        */

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_instance->GetSurface();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = m_device->FindQueueFamilies(); // findQueueFamilies(m_device->GetPhysicalDevice());
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if(indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }else{
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if(vkCreateSwapchainKHR(m_device->Get(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        {
            KN_ERROR("failed to create swapchain");
        }

        m_swapChainImageFormat = surfaceFormat.format;
        m_swapChainExtent = extent;
    }

    void Swapchain::GetSwapchainImages()
    {
        KN_INSTRUMENT_FUNCTION()
        uint32_t imageCount;
        vkGetSwapchainImagesKHR(m_device->Get(), m_swapChain, &imageCount, nullptr);
        m_swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_device->Get(), m_swapChain, &imageCount, m_swapChainImages.data());
    }

    void Swapchain::CreateImageViews()
    {
        KN_INSTRUMENT_FUNCTION()
        m_swapChainImageViews.resize(m_swapChainImages.size());

        for (uint32_t i = 0; i < m_swapChainImages.size(); i++) {
            m_swapChainImageViews[i] = new ImageView(m_device, m_swapChainImages[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1); 
        }
    }

    void Swapchain::CreateColorResources()
    {
        KN_INSTRUMENT_FUNCTION()
        VkFormat colorFormat = m_swapChainImageFormat;
        
        m_colorImage = new Image(m_device, m_swapChainExtent.width, m_swapChainExtent.height, 1, m_device->GetPhysicalDeviceProperties().mssaSamples,
          colorFormat,
           VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        m_colorImageView = new ImageView(m_device, m_colorImage->Get(), colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void Swapchain::CreateDepthResources()
    {
        VkFormat depthFormat = Image::FindSupportedFormat( m_device,
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

        m_depthImage = new Image(m_device, m_swapChainExtent.width, m_swapChainExtent.height, 1,
                             m_device->GetPhysicalDeviceProperties().mssaSamples, depthFormat,
                             VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        m_depthImageView = new ImageView(m_device, m_depthImage->Get(), depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
		// m_depthImageView.transitionImageLayout();
		m_depthImage->TransitionImageLayout(m_commandPool, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
		

        // CreateImage(m_swapChainExtent.width, m_swapChainExtent.height, 1, m_device->GetPhysicalDeviceProperties().mssaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depthImage, m_depthImageMemory);
        // m_depthImageView = createImageView(m_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
        // transitionImageLayout(m_depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
    }

    void Swapchain::CreateFramebuffers()
    {
        KN_INSTRUMENT_FUNCTION()
		if(m_renderPass == nullptr) KN_WARN("renderpass is nullptr");
        m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

        for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
		{
            std::array<VkImageView, 3> attachments = {
                m_colorImageView->Get(),
                m_depthImageView->Get(),
                m_swapChainImageViews[i]->Get()
            };
            
            m_swapChainFramebuffers[i] = new Framebuffer(m_device, m_swapChainExtent.width, m_swapChainExtent.height,
                                                     m_renderPass->Get(), attachments.data(), 3);
        }
    }

    void Swapchain::DestroySwapchain()
    {
        KN_INSTRUMENT_FUNCTION()
        vkDeviceWaitIdle(m_device->Get());
		

		delete m_depthImage;
		delete m_depthImageView;

		delete m_colorImage;
		delete m_colorImageView;

		for (auto imageView : m_swapChainImageViews)
		{
			delete imageView;
		}

        for (auto framebuffer : m_swapChainFramebuffers)
		{
			delete framebuffer;
        }

        vkDestroySwapchainKHR(m_device->Get(), m_swapChain, nullptr);
    }

	void Swapchain::BindRenderPass(RenderPass *renderPass)
	{
		m_renderPass = renderPass;
	}
}


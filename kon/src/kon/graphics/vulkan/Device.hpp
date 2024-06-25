
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <vector>

#include <kon/graphics/vulkan/Instance.hpp>
#include <kon/core/Logging.hpp>

namespace kon
{
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    enum class DeviceSelectionMethod
    {
        FirstCompatible,
        Scored
    };

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    // Move to swapchain at some point
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct PhysicalDeviceProperties
    {
        VkSampleCountFlagBits mssaSamples;
        uint32_t apiVersion;
        uint32_t driverVersion;
        char *deviceName;
    };

    class Device
    {
    public:
        Device(Instance *instance);
        ~Device();

        VkDevice Get() { return m_device; }

        PhysicalDeviceProperties &GetPhysicalDeviceProperties() { return m_deviceProperties; }
        VkPhysicalDevice GetPhysicalDevice() { return m_physicalDevice; }
		VkQueue GetGraphicsQueue() { return m_graphicsQueue; }
		VkQueue GetPresentQueue() { return m_presentQueue; }

        SwapChainSupportDetails QuerySwapChainSupport() { return QuerySwapChainSupport(m_physicalDevice); }
        QueueFamilyIndices FindQueueFamilies() { return FindQueueFamilies(m_physicalDevice); }
        bool CheckDeviceExtensionSupport() { return CheckDeviceExtensionSupport(m_physicalDevice); }
        bool IsDeviceSuitable() { return IsDeviceSuitable(m_physicalDevice); }

    private:
        void CreatePhysicalDevice();
        void CreateLogicalDevice();
		void CreateDeviceQueue();
        void PopulatePhysicalDeviceProperties();

        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        bool IsDeviceSuitable(VkPhysicalDevice device);

        VkPhysicalDevice PickDeviceFunctionFirstCompatible(std::vector<VkPhysicalDevice> devices);
        VkPhysicalDevice PickDeviceFunctionScored(std::vector<VkPhysicalDevice> devices);

    private:
        VkPhysicalDevice m_physicalDevice {VK_NULL_HANDLE};
        VkDevice m_device;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;
        Instance *m_instance;
        PhysicalDeviceProperties m_deviceProperties;
        QueueFamilyIndices m_queueFamilyIndices;
    };
}

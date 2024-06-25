
#include "Device.hpp"

#include <vector>
#include <map>
#include <set>
#include <string>

#include <kon/core/Logging.hpp>
#include <kon/debug/Debug.hpp>

namespace kon
{
    QueueFamilyIndices Device::FindQueueFamilies(VkPhysicalDevice device)
    {
        KN_INSTRUMENT_FUNCTION()

        QueueFamilyIndices indices;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto &queueFamily : queueFamilies)
        {
            if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_instance->GetSurface(), &presentSupport);

            if(presentSupport)
            {
                indices.presentFamily = i;
            }

            if(indices.isComplete())
            {
                break;
            }

            i++;
        }

        return indices;
    }

    SwapChainSupportDetails Device::QuerySwapChainSupport(VkPhysicalDevice device)
    {
        KN_INSTRUMENT_FUNCTION()

        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_instance->GetSurface(), &details.capabilities);
        
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_instance->GetSurface(), &formatCount, nullptr);
        if(formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_instance->GetSurface(), &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_instance->GetSurface(), &presentModeCount, nullptr);
        if(presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_instance->GetSurface(), &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    bool Device::CheckDeviceExtensionSupport(VkPhysicalDevice device)
    {
        KN_INSTRUMENT_FUNCTION()

        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requriedExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for(const auto &extension : availableExtensions)
        {
            requriedExtensions.erase(extension.extensionName);
        }

        return requriedExtensions.empty();
    }

    bool Device::IsDeviceSuitable(VkPhysicalDevice device)
    {
        KN_INSTRUMENT_FUNCTION()
        QueueFamilyIndices indices = FindQueueFamilies(device);

        bool extensionsSupported = CheckDeviceExtensionSupport(device);
        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return m_queueFamilyIndices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    VkPhysicalDevice Device::PickDeviceFunctionFirstCompatible(std::vector<VkPhysicalDevice> devices)
    {
        KN_INSTRUMENT_FUNCTION()

        for(const auto &device : devices)
        {
            if(IsDeviceSuitable(device));
            {
                m_queueFamilyIndices = FindQueueFamilies(device);
                return device;
            }
        }
    }

    VkPhysicalDevice PickDeviceFunctionFirstCompatible(std::vector<VkPhysicalDevice> devices)
    {
        KN_WARN("PickDeviceFunctionFirstCompatible not implemented yet");
    }

    Device::Device(Instance *instance)
        : m_instance(instance)
    {
        KN_INSTRUMENT_FUNCTION()

        CreatePhysicalDevice();
        PopulatePhysicalDeviceProperties();
        CreateLogicalDevice();
		CreateDeviceQueue();

        KN_INFO("Device selected (%s)\n api version %u\n driver version %u",
            m_deviceProperties.deviceName,
            m_deviceProperties.apiVersion,
            m_deviceProperties.driverVersion);
    }

    Device::~Device()
    {
        KN_INSTRUMENT_FUNCTION()

        vkDestroyDevice(m_device, nullptr);
    }

    void Device::CreatePhysicalDevice()
    {
        KN_INSTRUMENT_FUNCTION()

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance->Get(), &deviceCount, nullptr);
        if(deviceCount == 0) KN_ERROR("There are zero devices that support vulkan on this machine");

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_instance->Get(), &deviceCount, devices.data());

        m_physicalDevice = PickDeviceFunctionFirstCompatible(devices);
    }

    void Device::PopulatePhysicalDeviceProperties()
    {
        KN_INSTRUMENT_FUNCTION()

        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(m_physicalDevice, &physicalDeviceProperties);

        m_deviceProperties.mssaSamples = VK_SAMPLE_COUNT_8_BIT;
        m_deviceProperties.apiVersion = physicalDeviceProperties.apiVersion;
        m_deviceProperties.driverVersion = physicalDeviceProperties.driverVersion;
        m_deviceProperties.deviceName = &physicalDeviceProperties.deviceName[0];
    }

    void Device::CreateLogicalDevice()
    {
        KN_INSTRUMENT_FUNCTION()
        // we only want one queue with graphics capabilities
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {m_queueFamilyIndices.graphicsFamily.value(), m_queueFamilyIndices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        /*
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;

        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        */

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        deviceFeatures.sampleRateShading = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;

        // you set this even though you dont have to for backwards compatability
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if(KN_ENABLE_VALIDATION)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(Instance::s_validationLayers.size());
            createInfo.ppEnabledLayerNames = Instance::s_validationLayers.data();
        }else{
            createInfo.enabledLayerCount = 0;
        }

        if(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
        {
            KN_ERROR("failed to create logical device");
        }
    }

	void Device::CreateDeviceQueue()
	{
		vkGetDeviceQueue(m_device, m_queueFamilyIndices.graphicsFamily.value(), 0, &m_graphicsQueue);
		vkGetDeviceQueue(m_device, m_queueFamilyIndices.presentFamily.value(), 0, &m_presentQueue);
	}
}

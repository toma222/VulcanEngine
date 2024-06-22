
#include "Instance.hpp"

#include <cassert>
#include <vector>

#include <kon/core/Logging.hpp>
#include <kon/graphics/Graphics.hpp>
#include <kon/debug/Debug.hpp>

namespace kon
{
    std::vector<const char*> Instance::s_validationLayers  = 
    {
        "VK_LAYER_KHRONOS_validation"
    };

    std::vector<const char*> GetRequiredExtensions()
    {
        KN_INSTRUMENT_FUNCTION()

        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        if(KN_ENABLE_VALIDATION)
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    bool CheckValidationLayerSupport()
    {
        KN_INSTRUMENT_FUNCTION()

        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for(const char *layerName : Instance::s_validationLayers)
        {
            bool layerFound = false;

            for(const auto &layerProperties : availableLayers)
            {
                if(strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if(!layerFound) return false;
        }

        return true;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {
        
        KN_INSTRUMENT_FUNCTION()

        KN_INFO("%s", pCallbackData->pMessage);

        return VK_FALSE;
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        KN_INSTRUMENT_FUNCTION()

        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    Instance::Instance(GLFWwindow *window)
    {
        KN_INSTRUMENT_FUNCTION()

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        if(KN_ENABLE_VALIDATION && !CheckValidationLayerSupport())
        {
            assert("Vulkan validation layers requested, but not available");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan Engine";
        appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.pEngineName = "kon";
        appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        auto reqExtensions = GetRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(reqExtensions.size());
        createInfo.ppEnabledExtensionNames = reqExtensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if(KN_ENABLE_VALIDATION)
        {
            KN_INFO("Creating validation layers");
            createInfo.enabledLayerCount = static_cast<uint32_t>(Instance::s_validationLayers.size());
            createInfo.ppEnabledLayerNames = Instance::s_validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);

            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        }else{
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
        if(result != VK_SUCCESS)
        {
            assert("vkCreateInstance did not return success");
        }

        CreateDebugUtils();

        if(glfwCreateWindowSurface(m_instance, window, nullptr, &m_surface) != VK_SUCCESS)
        {
            KN_WARN("failed to create window surface");
        }

        m_valid = true;
    }

    void Instance::CreateDebugUtils()
    {
        KN_INSTRUMENT_FUNCTION()
        
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        populateDebugMessengerCreateInfo(debugCreateInfo);

        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(m_instance, &debugCreateInfo, nullptr, &m_debugMessenger);
        } else {
            KN_WARN("CreateDebugUtilsMessengerEXT failed");
        }
    }

    void Instance::DestroyDebugUtils()
    {
        KN_INSTRUMENT_FUNCTION()

        if(KN_ENABLE_VALIDATION)
        {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr)
                func(m_instance, m_debugMessenger, nullptr);
        }
    }

    Instance::~Instance()
    {
        KN_INSTRUMENT_FUNCTION()

        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        vkDestroyInstance(m_instance, nullptr);
    }
}

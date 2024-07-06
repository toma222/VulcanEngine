
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#ifdef KN_DEBUG
    #define KN_ENABLE_VALIDATION true
#else
    #define KN_ENABLE_VALIDATION false
#endif

namespace kon
{
    class Instance
    {
    public:
        static std::vector<const char*> s_validationLayers;

    public:
        Instance(GLFWwindow *window);
        ~Instance();

        VkInstance Get() { return m_instance; }
        bool Valid() { return m_valid; }

        void DestroyDebugUtils();
        VkSurfaceKHR GetSurface() { return m_surface; }
    
    private:
        void CreateDebugUtils();

    public:
        VkInstance m_instance;
        VkSurfaceKHR m_surface;
        VkDebugUtilsMessengerEXT  m_debugMessenger;
        bool m_valid { false };
    };
}

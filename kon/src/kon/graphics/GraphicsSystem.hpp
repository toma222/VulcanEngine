
#pragma once

#include <kon/core/Object.hpp>

#include <kon/graphics/vulkan/Instance.hpp>
#include <kon/graphics/vulkan/Device.hpp>
#include <kon/graphics/vulkan/Swapchain.hpp>
#include <kon/graphics/vulkan/commands/CommandPool.hpp>

#include <kon/graphics/pipelines/Pipeline.hpp>
#include <kon/graphics/pipelines/PipelineShaded.hpp>

#include <GLFW/glfw3.h>

#include <kon/core/System.hpp>

namespace kon
{
    /*
        holds the instance, device, swapchain, and command pool
    */
		
	constexpr const char *GRAPHICS_SYSTEM_PATH = "systems/graphics/";

    class GraphicsSystem : public System
    {
    KN_OBJECT(GraphicsSystem, System)

    public:
        GraphicsSystem(Context *context, GLFWwindow *window);
        ~GraphicsSystem() override;

        void Update() override;
		static void RegisterResources(Context *context, LoadResourceArray &lra);

        // void DrawFrame();

    private:
        Instance *m_vulkanInstance;
        Device *m_device;
        CommandPool *m_commandPool;
        Swapchain *m_swapchain;

        Pipeline *m_pipeline;

        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
        uint32_t m_currentFrame {0};
    };
}

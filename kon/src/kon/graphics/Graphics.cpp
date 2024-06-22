
#include "Graphics.hpp"
#include "kon/core/Logging.hpp"
#include "kon/core/Core.hpp"

#include <cstdio>
#include <cassert>

#include "vulkan/VulkanGraphicsCommands.hpp"

namespace kon
{
    GraphicsCommands *Graphics::s_graphics = static_cast<GraphicsCommands*>(new VulkanGraphicsCommands());

    void GraphicsCommands::BindWindow(GLFWwindow *window)
    {
        m_window = window;
    }

    void GraphicsCommands::UnbindWindow()
    {
        m_window = nullptr;
    }

    void Graphics::Init(GraphicsAPI api, GLFWwindow *window)
    {
        // s_graphics->Init(window);
    }

    void Graphics::DrawFrame()
    {
        s_graphics->DrawFrame();
    }

    void Graphics::Clean()
    {
        if(s_graphics == nullptr) assert("clean called when s_graphics==nullptr");
        s_graphics->Clean();
    }
}

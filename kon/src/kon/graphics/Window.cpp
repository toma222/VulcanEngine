
#include "Window.hpp"

#include "kon/core/Logging.hpp"
#include "Graphics.hpp"

#include <kon/debug/Debug.hpp>

namespace kon
{
    Window::Window(Context *context, int width, int height, const char *name)
        : m_name(name), m_width(width), m_height(height)
    {
        KN_INSTRUMENT_FUNCTION()
        KN_TRACE("Creating window");

        if(!glfwInit())
        {
            KN_TRACE("an error has happened");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        m_window = glfwCreateWindow(500, 500, "Vulkan window", nullptr, nullptr);

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int w, int h)
        {
            Graphics::Get()->Viewport(0, 0, w, h);
        });
    }

    Window::~Window()
    {
        KN_INSTRUMENT_FUNCTION()
        KN_TRACE("Destroying window");
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::PollEvents()
    {
        KN_INSTRUMENT_FUNCTION()
        glfwPollEvents();
    }

    bool Window::ShouldWindowClose()
    {
        return glfwWindowShouldClose(m_window);
    }
}

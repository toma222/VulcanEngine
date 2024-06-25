
#pragma once

#include <kon/core/Object.hpp>
#include <kon/types/Vector2.hpp>

#include <kon/core/Context.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kon
{
    class Window : public Object
    {
    KN_OBJECT(Window, Object)

    public:
        Window(Context *context, int width, int height, const char *name);
        ~Window();

        void PollEvents();
        bool ShouldWindowClose();

        Vector2Int GetDimensions() { return {m_width, m_height}; }

        GLFWwindow *GetHandle() const { return m_window; }

    private:
        const String m_name;
        int m_width;
        int m_height;

        GLFWwindow *m_window;
    };
}

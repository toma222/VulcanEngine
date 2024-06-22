
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifdef KN_DEBUG
    #define KN_ENABLE_VALIDATION 1
#else
    #define KN_ENABLE_VALIDATION 0
#endif

namespace kon
{
    enum class GraphicsAPI
    {
        NONE,
        VULKAN
    };

    class GraphicsCommands
    {
    public:
        virtual void Init() = 0;
        virtual void Clean() = 0;
        virtual void DrawFrame() = 0;
        virtual void Viewport(int x, int y, int w, int h) = 0;

        void BindWindow(GLFWwindow *window);
        void UnbindWindow();

    protected:
        GLFWwindow *m_window {nullptr};
    };

    class Graphics
    {
    public:
        static void Init(GraphicsAPI api, GLFWwindow *window);
        static void Clean();
        static void DrawFrame();
        static GraphicsCommands *Get() { return s_graphics; }

    private:
        static GraphicsCommands *s_graphics;
    };
}

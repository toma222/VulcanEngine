
#include "Application.hpp"

#include "kon/core/Logging.hpp"
#include "kon/graphics/Graphics.hpp"
#include <kon/debug/Debug.hpp>

namespace kon
{
    Application::Application(Context *context)
        : m_context(context)
    {
        KN_INSTRUMENT_FUNCTION();
        KN_TRACE("Application created");
        m_window = new Window(context, 500, 500, "Vulkan Window");
        // Graphics::Get()->BindWindow(m_window->GetHandle());
        // Graphics::Get()->Init();
        // Graphics::Init(GraphicsAPI::VULKAN);
        m_engine = new Engine(context);

        m_graphicsSystem = new GraphicsSystem(context, m_window->GetHandle());
        context->AddSystem(static_cast<System*>(m_graphicsSystem));
    }

    Application::~Application()
    {
        KN_INSTRUMENT_FUNCTION()
        KN_TRACE("Application destroyed");
        // Graphics::Clean();
        delete m_window;
        delete m_engine;
    }

    int Application::Run()
    {
        KN_TRACE("Application::Run() called");
        Awake();

        KN_TRACE("Application::Run() entering main loop");
        while(!m_engine->IsClosing())
        {
            KN_INSTRUMENT_SCOPE("App frame", 0);

            m_window->PollEvents();
            m_engine->UpdateFrame();
            // Graphics::DrawFrame();

            if(m_window->ShouldWindowClose())
            {
                EventApplicationExit e;
                m_context->EmitEvent(e);
            }
        }

        KN_TRACE("Application::Run() loop exited");

        return 0;
    }
}

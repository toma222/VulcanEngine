
#include "Application.hpp"

#include "kon/core/Logging.hpp"
#include <kon/debug/Debug.hpp>

#include <kon/resource/Resource.hpp>
#include <kon/resource/ResourceImage.hpp>
#include <kon/resource/ResourceModel.hpp>
#include <kon/resource/ResourceRawfile.hpp>

namespace kon
{
    Application::Application(Context *context)
        : m_context(context)
    {
        KN_INSTRUMENT_FUNCTION();
        KN_TRACE("Application created");
        m_window = new Window(context, 500, 500, "Vulkan Window");
        m_engine = new Engine(context);

        // Get resources and load them
        LoadResourceArray lra;
        lra.AddResource(static_cast<Resource*>(new ResourceImage()), "textures/viking_room.png");
        lra.AddResource(static_cast<Resource*>(new ResourceModel()), "models/viking_room.obj");
        lra.AddResource(static_cast<Resource*>(new ResourceRawfile()), "shaders/vert.spv");
        lra.AddResource(static_cast<Resource*>(new ResourceRawfile()), "shaders/frag.spv");
        lra.LoadResourceToContext(context);

        

        m_graphicsSystem = new GraphicsSystem(context, m_window->GetHandle());
        context->AddSystem(static_cast<System*>(m_graphicsSystem));
    }

    Application::~Application()
    {
        KN_INSTRUMENT_FUNCTION()
        KN_TRACE("Application destroyed");
        // Graphics::Clean();
        delete m_engine;
        delete m_window;
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

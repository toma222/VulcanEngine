
#include "Engine.hpp"
#include "kon/core/Logging.hpp"
#include "kon/events/ApplicationEvent.hpp"
#include <kon/debug/Debug.hpp>

namespace kon
{
    Engine::Engine(Context *context)
        : m_context(context)
    {
        KN_INSTRUMENT_FUNCTION()
        KN_TRACE("Engine created");
        context->AddListener(static_cast<Object*>(this), (i16)EventClass::Application);
        // init submodules here
    }

    Engine::~Engine()
    {
        KN_INSTRUMENT_FUNCTION()
        KN_TRACE("Engine destroyed");
        m_context->ClearSystems();
        // delete all the submodules ig
    }

    void Engine::UpdateFrame()
    {
        KN_INSTRUMENT_FUNCTION()

        // update the context systems
        m_context->SystemUpdate();
    }

    void Engine::OnEvent(Event &event)
    {
        KN_INSTRUMENT_FUNCTION()
        switch(event.GetEventType())
        {
            case(EventType::EventApplicationExit): m_close = true;

            default: return;
        }
    }
}

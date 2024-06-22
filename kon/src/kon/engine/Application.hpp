
#pragma once

#include "Engine.hpp"
#include "kon/core/Object.hpp"
#include "kon/graphics/Window.hpp"
#include "kon/core/Context.hpp"

namespace kon
{
    class knAPI Application : public Object
    {
    KN_OBJECT(Application, Object)

    public:
        Application(Context *context);
        virtual ~Application();

        virtual void Awake() {}
        virtual void Start() {}
        virtual void Clean() {}

        int Run();

    private:
        Engine *m_engine;
        Context *m_context;
        Window *m_window;
    }; 
}

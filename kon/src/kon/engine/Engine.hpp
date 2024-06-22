
#pragma once

#include "kon/core/Object.hpp"
#include "kon/core/Context.hpp"

namespace kon
{
    class knAPI Engine : public Object
    {
    KN_OBJECT(Engine, Object)

    public:
        Engine(Context *context);
        ~Engine();

        bool IsClosing() { return m_close; }

        void UpdateFrame();

        void OnEvent(Event &event) override;

    private:
        Context *m_context;
        bool m_close {false};
    };
}


#pragma once

#include <kon/core/Object.hpp>
#include <kon/types/String.hpp>
#include <kon/graphics/pipelines/Pipeline.hpp>

namespace kon
{
    struct GraphicsContextSpecs
    {
        bool headless {false};
    };

    // The global graphics context that will be stored in context
    // TODO i have been struck with a bolt of inspiration; please hold
    class GraphicsContext : public Object
    {
    public:
        GraphicsContext();
        ~GraphicsContext();

        

    private:
        ArrayList<Pipeline*> m_pipelines;
    };
}

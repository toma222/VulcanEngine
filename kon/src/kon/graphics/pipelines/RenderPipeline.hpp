
#pragma once

#include <kon/core/Object.hpp>

namespace kon
{
    class RenderPipeline : public Object
    {
    KN_OBJECT(RenderPipeline, Object)

    public:
        struct PipelineDebugInfo
        {
            // idk know what to put here
        };

    public:
        RenderPipeline() = default;
        virtual ~RenderPipeline() = default;

        virtual void Draw() = 0;

        virtual PipelineDebugInfo &GetDebugInfo() {};
    };
}


#pragma once

#include <kon/core/Object.hpp>

#include <kon/graphics/vulkan/commands/CommandPool.hpp>
#include <kon/graphics/vulkan/image/Framebuffer.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kon
{
    struct RenderFrameData
    {
        u32 swapchainImage;
        VkExtent2D renderExtent;
    };

    class Renderer : public Object
    {
    KN_OBJECT(Renderer, Object)

    public:
        Renderer() = default;
        virtual ~Renderer() = default;

        virtual void GetDrawCommands(CommandBuffer *buffer, const RenderFrameData &data) = 0;

        void SetRenderTarget(Framebuffer *target);
        const Framebuffer *GetRenderTarget() const;

        void SetPushBuffer(void *data, u32 size=128);
        const void *GetPushBuffer() const;
        const u32 GetPushBufferSize() const;

    protected:
        Framebuffer *m_renderTarget { nullptr };

        void *m_pushBuffer;
        u32 m_pushBufferSize;
    };
}

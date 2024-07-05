
#pragma once

#include <kon/core/Object.hpp>

#include <kon/graphics/vulkan/commands/CommandPool.hpp>
#include <kon/graphics/vulkan/image/Framebuffer.hpp>
#include <kon/graphics/vulkan/Swapchain.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kon
{
    struct RenderFrameData
    {
        u32 swapchainImage;
        VkExtent2D renderExtent;
    };

    // Creates a vk pipeline that manages
    class Pipeline : public Object
    {
    KN_OBJECT(Pipeline, Object)

    public:
        Pipeline() = default;
        virtual ~Pipeline() = default;

        virtual void GetDrawCommands(VkCommandBuffer buffer, const RenderFrameData &data) = 0;
        virtual void Recreate(Swapchain *swapchain) = 0;

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

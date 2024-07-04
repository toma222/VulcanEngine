
#include "Renderer.hpp"

namespace kon
{
    void Renderer::SetRenderTarget(Framebuffer *target)
    {
        m_renderTarget = target;
    }

    const Framebuffer *Renderer::GetRenderTarget() const
    {
        return m_renderTarget;
    }

    void Renderer::SetPushBuffer(void *data, u32 size=128)
    {
        m_pushBufferSize = size;
        m_pushBuffer = data;
    }

    const void *Renderer::GetPushBuffer() const
    {
        return m_pushBuffer;
    }

    const u32 Renderer::GetPushBufferSize() const
    {
        return m_pushBufferSize;
    }
}


#include "Pipeline.hpp"

namespace kon
{
    void Pipeline::SetRenderTarget(Framebuffer *target)
    {
        m_renderTarget = target;
    }

    const Framebuffer *Pipeline::GetRenderTarget() const
    {
        return m_renderTarget;
    }

    void Pipeline::SetPushBuffer(void *data, u32 size)
    {
        m_pushBufferSize = size;
        m_pushBuffer = data;
    }

    const void *Pipeline::GetPushBuffer() const
    {
        return m_pushBuffer;
    }

    const u32 Pipeline::GetPushBufferSize() const
    {
        return m_pushBufferSize;
    }
}

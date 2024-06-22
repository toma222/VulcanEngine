
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <kon/graphics/vulkan/Device.hpp>
#include <kon/graphics/vulkan/pipeline/Attachment.hpp>

namespace kon
{
    // a way to define attachments
    //      - and their refrences


    class RenderPass
    {
    public:
        RenderPass(Device *device, AttachmentArray attachments);
        ~RenderPass();

        VkRenderPass Get() { return m_renderPass; }

    private:
        VkRenderPass m_renderPass;
        Device *m_device;
    };
}

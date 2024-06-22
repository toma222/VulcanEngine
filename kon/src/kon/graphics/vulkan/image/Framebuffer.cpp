
#include "Framebuffer.hpp"

namespace kon
{
    Framebuffer::Framebuffer(Device *device, uint32_t width, uint32_t height,
                    VkRenderPass renderPass, const VkImageView *attachments, uint32_t attachmentSize)
        : m_width(width), m_height(height), m_device(device)
    {
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = attachmentSize;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = width;
        framebufferInfo.height = height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(m_device->Get(), &framebufferInfo, nullptr, &m_framebuffer) != VK_SUCCESS) {
            KN_WARN("failed to create framebuffer!");
        }
    }

    Framebuffer::~Framebuffer()
    {

    }
}


#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <kon/graphics/vulkan/Instance.hpp>
#include <kon/graphics/vulkan/Device.hpp>
#include <kon/graphics/vulkan/image/ImageView.hpp>

namespace kon
{
    class Framebuffer
    {
    public:
        Framebuffer(Device *device, uint32_t width, uint32_t height,
                    VkRenderPass renderPass, const VkImageView *attachments, uint32_t attachmentSize);
        Framebuffer() = default;
        ~Framebuffer();

        uint32_t GetWidth() { return m_width; }
        uint32_t GetHeight() { return m_height; }

		VkFramebuffer Get() { return m_framebuffer; }

		void Destroy();

    private:
        VkFramebuffer m_framebuffer { VK_NULL_HANDLE };
        uint32_t m_width;
        uint32_t m_height;

        Device *m_device { nullptr };
    };
}

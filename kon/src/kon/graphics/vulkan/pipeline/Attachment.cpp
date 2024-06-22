
#include "Attachment.hpp"

namespace kon
{
    Attachment::Attachment(Device *device, VkFormat format, uint32_t index)
        : m_device(device), m_format(format), m_index(index) {}

    VkAttachmentReference AttachmentColor::GetReference()
    {
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = m_index;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }

    VkAttachmentDescription AttachmentColor::GetDescription()
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = m_format;
        colorAttachment.samples = m_device->GetPhysicalDeviceProperties().mssaSamples;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        return colorAttachment;
    }

    VkAttachmentReference AttachmentDepth::GetReference() {}
    VkAttachmentDescription AttachmentDepth::GetDescription() {}

    VkAttachmentReference AttachmentColorResolve::GetReference() {}
    VkAttachmentDescription AttachmentColorResolve::GetDescription() {}

    AttachmentArray::AttachmentArray(Device *device, VkFormat format)
    {
        AddColorAttachment(new AttachmentColor{device, format, 0});
        AddDepthAttachment(new AttachmentDepth{device, format, 1});
        AddColorResAttachment(new AttachmentColorResolve{device, format, 2});
    }

    void AttachmentArray::AddColorAttachment(Attachment *attachment)
    {
        m_descriptions.push_back(attachment->GetDescription());
        m_colorRef = attachment->GetReference();
    }

    void AttachmentArray::AddDepthAttachment(Attachment *attachment)
    {
        m_descriptions.push_back(attachment->GetDescription());
        m_depthAttachmentRef = attachment->GetReference();
    }

    void AttachmentArray::AddColorResAttachment(Attachment *attachment)
    {
        m_descriptions.push_back(attachment->GetDescription());
        m_colorResolveRef = attachment->GetReference();
    }
}


#include "Attachment.hpp"
#include "kon/core/Logging.hpp"
#include "kon/graphics/vulkan/image/Image.hpp"

namespace kon
{
    AttachmentArray::AttachmentArray(Device *device, VkFormat format)
    {
        // AddColorAttachment(new AttachmentColor{device, format, 0});
        // AddDepthAttachment(new AttachmentDepth{device, format, 1});
        // AddColorResAttachment(new AttachmentColorResolve{device, format, 2});


		VkAttachmentDescription colorAttachment{};
        colorAttachment.format = format;
        colorAttachment.samples = device->GetPhysicalDeviceProperties().mssaSamples;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;



        // VkAttachmentReference colorAttachmentRef{};
        m_colorRef.attachment = 0;
        m_colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachmentResolve{};
        colorAttachmentResolve.format = format;
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentDescription depthAttachment{};
		VkFormat depthFormat = Image::FindSupportedFormat( device,
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );


        depthAttachment.format = depthFormat;
        depthAttachment.samples = device->GetPhysicalDeviceProperties().mssaSamples;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


        // VkAttachmentReference depthAttachmentRef{};
        m_depthRef.attachment = 1;
        m_depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        // VkAttachmentReference colorAttachmentResolveRef{};
        m_colorResolveRef.attachment = 2;
        m_colorResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        // std::array<VkAttachmentDescription, 3> attachments = {colorAttachment, depthAttachment, colorAttachmentResolve};
		m_descriptions[0] = colorAttachment;
		m_descriptions[1] = depthAttachment;
		m_descriptions[2] = colorAttachmentResolve;

    }
}

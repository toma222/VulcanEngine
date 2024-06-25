
#include "RenderPass.hpp"
#include "vulkan/vulkan_core.h"

#include <array>

#include <kon/core/Logging.hpp>

namespace kon
{
    RenderPass::RenderPass(Device *device, AttachmentArray attachments)
        : m_device(device)
    {
		KN_TRACE("here");

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        // dependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &attachments.m_colorRef;
        subpass.pResolveAttachments = &attachments.m_colorResolveRef;
        subpass.pDepthStencilAttachment = &attachments.m_depthRef;

        // std::array<VkAttachmentDescription, 3> attachments = {colorAttachment, depthAttachment, colorAttachmentResolve};

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = attachments.m_descriptions.size();
        renderPassInfo.pAttachments = attachments.m_descriptions.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(m_device->Get(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
            KN_WARN("failed to create render pass!");
        }
    }

	RenderPass::~RenderPass()
	{
		vkDestroyRenderPass(m_device->Get(), m_renderPass, nullptr);
	}
}

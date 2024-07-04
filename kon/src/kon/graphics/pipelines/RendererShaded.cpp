
#include "RendererShaded.hpp"

#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #include ""

namespace kon
{
    RendererShaded::RendererShaded(Context *context, Device *device, Swapchain *swapchain, CommandPool *commandPool)
    {
        // Create render pass
        CreateRenderPass(device, swapchain, commandPool);

        // Create framebuffers
        // m_framebuffers.Reserve();

        // Create vertex and index buffers

        // Create Descriptor Pool

        // Create Descriptor Set

        // Create Descriptors
    }

    RendererShaded::~RendererShaded()
    {

    }

    inline void RendererShaded::CreateRenderPass(Device *device, Swapchain *swapchain, CommandPool *commandPool)
    {
        CreateRenderPassResources(device, swapchain, commandPool);
        m_renderPass = new RenderPass(device, AttachmentArray(device, swapchain->GetSwapchainFormat()));
    }

    inline void RendererShaded::CreateRenderPassResources(Device *device, Swapchain *swapchain, CommandPool *commandPool)
    {
        // --==========================--
        //        COLOR RESOURCES
        // --==========================--
        VkFormat colorFormat = swapchain->GetSwapchainFormat();
        
        m_renderPassResources.m_colorImage = new Image(device, swapchain->GetSwapchainExtent().width, swapchain->GetSwapchainExtent().height, 1, device->GetPhysicalDeviceProperties().mssaSamples,
          colorFormat,
           VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        m_renderPassResources.m_colorImageView = new ImageView(
            device, m_renderPassResources.m_colorImage->Get(), colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1
        );

        // --==========================--
        //        DEPTH RESOURCES
        // --==========================--
        VkFormat depthFormat = Image::FindSupportedFormat( device,
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

        m_renderPassResources.m_depthImage = new Image(device, swapchain->GetSwapchainExtent().width, swapchain->GetSwapchainExtent().height, 1,
                             device->GetPhysicalDeviceProperties().mssaSamples, depthFormat,
                             VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        m_renderPassResources.m_depthImageView = new ImageView(device, m_renderPassResources.m_depthImage->Get(),
                                            depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
		m_renderPassResources.m_depthImage->TransitionImageLayout(
            commandPool, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
    }

    void RendererShaded::GetDrawCommands(CommandBuffer *commandBuffer, const RenderFrameData &data)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer->Get(), &beginInfo) != VK_SUCCESS) {
            assert("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_renderPass->Get();
        renderPassInfo.framebuffer = m_renderTarget->Get(); // m_swapchain->GetFramebuffer(imageIndex)->Get(); // m_swapChainFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = data.renderExtent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer->Get(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        VkBuffer vertexBuffers[] = {m_vertexBuffer->Get()->Get()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer->Get(), 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer->Get(), m_indexBuffer->Get()->Get(), 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindPipeline(commandBuffer->Get(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_renderPipeline->Get());
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(data.renderExtent.width);
        viewport.height = static_cast<float>(data.renderExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer->Get(), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = data.renderExtent;
        vkCmdSetScissor(commandBuffer->Get(), 0, 1, &scissor);

        
		PushConstant c;
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		c.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		vkCmdPushConstants(commandBuffer->Get(), m_renderPipeline->GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &c);
			
        // vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0); // DRAW THE DAMN TRIANGLE
        vkCmdBindDescriptorSets(commandBuffer->Get(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_renderPipeline->GetLayout(), 0, 1, &m_descriptorSets->Get()[m_currentFrame], 0, nullptr);

        vkCmdDrawIndexed(commandBuffer->Get(), static_cast<uint32_t>(m_indicesCount), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer->Get());

        if (vkEndCommandBuffer(commandBuffer->Get()) != VK_SUCCESS)
		{
            assert("failed to record command buffer!");
        }
    }
}

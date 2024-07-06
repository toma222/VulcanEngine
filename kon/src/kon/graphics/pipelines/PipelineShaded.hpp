
#pragma once

#include <kon/graphics/pipelines/Pipeline.hpp>
#include <kon/core/Context.hpp>
#include <kon/core/Object.hpp>

#include "kon/graphics/vulkan/Swapchain.hpp"
#include "kon/graphics/vulkan/buffer/IndexBuffer.hpp"
#include "kon/graphics/vulkan/buffer/UniformBuffer.hpp"
#include "kon/graphics/vulkan/buffer/VertexBuffer.hpp"
#include "kon/graphics/vulkan/commands/CommandPool.hpp"
#include "kon/graphics/vulkan/descriptor/Descriptor.hpp"
#include "kon/graphics/vulkan/descriptor/DescriptorPool.hpp"
#include "kon/graphics/vulkan/descriptor/DescriptorSets.hpp"
#include "kon/graphics/vulkan/image/TextureImage.hpp"
#include "kon/graphics/vulkan/image/TextureSampler.hpp"
#include "kon/graphics/vulkan/pipeline/RenderPass.hpp"
#include "kon/graphics/vulkan/pipeline/RenderPipeline.hpp"
#include "kon/graphics/vulkan/pipeline/ShaderModule.hpp"

#include <glm/glm.hpp>

#include <vulkan/vulkan.hpp>

namespace kon
{
    class PipelineShaded : public Pipeline
    {
    KN_OBJECT(PipelineShaded, Pipeline)

    public:
        struct PushConstant
        {
            glm::mat4 model;
        };

        struct UniformBufferData
        {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 projection;
        };

        struct RenderPassResources
        {
            Image *m_colorImage;
            ImageView *m_colorImageView;
            Image *m_depthImage;
            ImageView *m_depthImageView;
        };

    public:
        PipelineShaded(Context *context, Device *device, Swapchain *swapchain, CommandPool *commandPool);
        ~PipelineShaded() override;

        void GetDrawCommands(VkCommandBuffer buffer, const RenderFrameData &data) override;
        void Recreate(Swapchain *swapchain) override;

        void DrawMesh();

    private:
        inline void CreateRenderPass(Device *device, Swapchain *swapchain, CommandPool *commandPool);
        inline void CreateRenderPassResources(Device *device, Swapchain *swapchain, CommandPool *commandPool);
        inline void CreateBuffers(Device *device, Swapchain *swapchain, CommandPool *commandpool);
        inline void CreateDescriptors(Device *device);

        inline void CreateRenderPipeline(Device *device, Swapchain *swapchain, CommandPool *commandPool);
        inline void CreateRenderPipelineLayout(Device *device);

        inline void CreateFramebuffers(Device *device, Swapchain *swapchain);

    private:
        inline void DestroyFramebuffers();

    private:
        u32 m_indiciesCount {};
        IndexBuffer *m_indexBuffer;
        VertexBuffer *m_vertexBuffer;

        TextureImage *m_textureImage;
        TextureSampler *m_textureSampler;
        UniformBuffer *m_uniformBuffer;

        DescriptorPool *m_descriptorPool;
        DescriptorLayout *m_descriptorLayout;
        DescriptorSets *m_descriptorSets;

        ShaderModule *m_fragmentShader;
        ShaderModule *m_vertexShader;

        ArrayList<Framebuffer*> m_framebuffers;
        RenderPass *m_renderPass;
        VkPipeline m_renderPipeline;
        VkPipelineLayout m_renderPipelineLayout;

        RenderPassResources m_renderPassResources;

    private:
        Device *m_device;
    };
}

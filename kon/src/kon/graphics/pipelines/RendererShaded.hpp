
#pragma once

#include <kon/graphics/pipelines/Renderer.hpp>
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

namespace kon
{
    class RendererShaded : public Renderer
    {
    KN_OBJECT(RendererShaded, Renderer)

    public:
        struct PushConstant
        {
            glm::mat4 model;
        };

        struct RenderPassResources
        {
            Image *m_colorImage;
            ImageView *m_colorImageView;
            Image *m_depthImage;
            ImageView *m_depthImageView;
        };

    public:
        RendererShaded(Context *context, Device *device, Swapchain *swapchain, CommandPool *commandPool);
        ~RendererShaded() override;

        void GetDrawCommands(CommandBuffer *buffer, const RenderFrameData &data) override;

        void DrawMesh();

    private:
        inline void CreateRenderPass(Device *device, Swapchain *swapchain, CommandPool *commandPool);
        inline void CreateRenderPassResources(Device *device, Swapchain *swapchain, CommandPool *commandPool);

    private:
        RenderPass *m_renderPass;
        ArrayList<Framebuffer> *m_framebuffers;

        IndexBuffer *m_indexBuffer;
        VertexBuffer *m_vertexBuffer;

        DescriptorLayout *m_descriptorLayout;
        DescriptorSets *m_descriptorSets;

        ShaderModule *m_fragmentShader;
        ShaderModule *m_vertexShader;

        RenderPipeline *m_renderPipeline;

        RenderPassResources m_renderPassResources;
    };
}


#include "PipelineShaded.hpp"

#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <kon/resource/ResourceModel.hpp>
#include <kon/resource/ResourceImage.hpp>
#include <kon/resource/ResourceRawfile.hpp>

#include <vulkan/vulkan.hpp>

// #include ""

namespace kon
{
    PipelineShaded::PipelineShaded(Context *context, Device *device, Swapchain *swapchain, CommandPool *commandPool)
        : m_context(context), m_device(device)
    {
        // Create render pass
        CreateRenderPass(device, swapchain, commandPool);

        // Create framebuffers
        CreateFramebuffers(device, swapchain);

        // Create all them buffers
        CreateBuffers(device, swapchain, commandPool);

        KN_INFO("Binding ubo");
        UniformBufferData ubo;
        ubo.model = glm::mat4(1.0f);
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.projection = glm::perspective(glm::radians(45.0f), swapchain->GetSwapchainExtent().width / (float) swapchain->GetSwapchainExtent().height, 0.1f, 10.0f);
        ubo.projection[1][1] *= -1;
    	m_uniformBuffer->CopyDataToBuffer(&ubo, sizeof(ubo));

        // Create Descriptors
        KN_INFO("Creating descriptors");
        CreateDescriptors(device);

        KN_INFO("layout");
        CreateRenderPipelineLayout(device);

        KN_INFO("renderpipeline");
        CreateRenderPipeline(device, swapchain, commandPool);

        KN_INFO("done");
    }

    void PipelineShaded::Recreate(Swapchain *swapchain)
    {
        KN_WARN("pipeline shaded recreate function not created yet");
    }

    PipelineShaded::~PipelineShaded()
    {
        DestroyFramebuffers();
		
        delete m_textureImage;

		delete m_uniformBuffer;

        delete m_textureSampler;

        delete m_descriptorPool;
		delete m_descriptorLayout;
		delete m_descriptorSets;

        delete m_indexBuffer;
		delete m_vertexBuffer;

        delete m_vertexShader;
		delete m_fragmentShader;

        delete m_renderPass;

        vkDestroyPipelineLayout(m_device->Get(), m_renderPipelineLayout, nullptr);
        vkDestroyPipeline(m_device->Get(), m_renderPipeline, nullptr);
    }

    inline void PipelineShaded::DestroyFramebuffers()
    {
        // Destroy all the framebuffer attachments
        delete m_renderPassResources.m_colorImage;
        delete m_renderPassResources.m_colorImageView;
        delete m_renderPassResources.m_depthImage;
        delete m_renderPassResources.m_depthImageView;

        // Destroy all the framebuffers
        for(int i = 0; i < m_framebuffers.Index(); i++)
        {
            delete m_framebuffers.Get(i);
        }

        // m_framebuffers.Reserve(0); // take it back to zero
    }

    inline void PipelineShaded::CreateDescriptors(Device *device)
    {
        DescriptonSizeFactory poolFactory;
            poolFactory.Add(DescriptorType::Uniform, 1);
            poolFactory.Add(DescriptorType::TextureSampler, 1);
		m_descriptorPool = new DescriptorPool(device, poolFactory);

		std::vector<DescriptorVector*> descriptors {};
		for (int i = 0; i < 1; i++)
		{
            // Defines one descriptor set (binded with different values)
			DescriptorVector *d = new DescriptorVector();
			d->Add<DescriptorUniform>(m_uniformBuffer); // this deppends on the frames in flight
			d->Add<DescriptorTextureSampler>(m_textureSampler, m_textureImage->GetImageView());
			descriptors.push_back(d);
		}

		m_descriptorLayout = new DescriptorLayout(device, descriptors);
		m_descriptorSets = new DescriptorSets(device, m_descriptorPool, m_descriptorLayout, 1);
    }

    inline void PipelineShaded::CreateFramebuffers(Device *device, Swapchain *swapchain)
    {
        const VkExtent2D scExtent = swapchain->GetSwapchainExtent();
        const u32 swapchainImages = swapchain->GetSwapchainImageViews().size();
        m_framebuffers.Reserve(swapchainImages);
        for(int i = 0; i < swapchainImages; i++)
        {
            // define the framebuffer attachments
            std::array<VkImageView, 3> attachments = {
                m_renderPassResources.m_colorImageView->Get(),
                m_renderPassResources.m_depthImageView->Get(),
                swapchain->GetSwapchainImageViews()[i]->Get()
            };

            m_framebuffers.Add(
                new Framebuffer(device, scExtent.width, scExtent.height,
                                m_renderPass->Get(), attachments.data(), 3)
            );
        }
    }

    inline void PipelineShaded::CreateBuffers(Device *device, Swapchain *swapchain, CommandPool *commandpool)
    {
        ResourceModel *model = static_cast<ResourceModel*>(m_context->GetResource("viking_room.obj"));
		auto &vertices = model->GetShape()->verticies;
		auto &indices = model->GetShape()->indicies;
        m_vertexBuffer = new VertexBuffer(device, commandpool, vertices.GetData(), vertices.Index() * sizeof(vertices.Get(0)));
		VertexDescription description(sizeof(ResourceModel::ModelVertex), 4);
			description.Add(ShaderType::Float3, offsetof(ResourceModel::ModelVertex, position));
			description.Add(ShaderType::Float3, offsetof(ResourceModel::ModelVertex, color));
			description.Add(ShaderType::Float2, offsetof(ResourceModel::ModelVertex, texCoord));
			description.Add(ShaderType::Float3, offsetof(ResourceModel::ModelVertex, normal));
		m_vertexBuffer->SetDescription(description);
		m_indexBuffer = new IndexBuffer(device, commandpool, indices.GetData(), indices.Index() * sizeof(indices.Get(0)));
        m_indiciesCount = indices.Index();

        ResourceImage *image = static_cast<ResourceImage*>(m_context->GetResource("viking_room.png"));
		int width = image->GetWidth();
		int height = image->GetHeight();
		// u8 *imageData = image->GetImageData();
		m_textureImage = new TextureImage(device, commandpool, image->GetImageData(),
				TextureData{width, height,
				static_cast<VkDeviceSize>(width * height * 4)});
        m_textureSampler = new TextureSampler(device);
        m_uniformBuffer = new UniformBuffer(device, commandpool, sizeof(UniformBufferData));

        ResourceRawfile *v = static_cast<ResourceRawfile*>(m_context->GetResource("vert.spv"));
		m_vertexShader = new ShaderModule(device, v->GetData(), v->GetSize());
	    ResourceRawfile *f = static_cast<ResourceRawfile*>(m_context->GetResource("frag.spv"));
		m_fragmentShader = new ShaderModule(device, f->GetData(), f->GetSize());
    }

    inline void PipelineShaded::CreateRenderPass(Device *device, Swapchain *swapchain, CommandPool *commandPool)
    {
        CreateRenderPassResources(device, swapchain, commandPool);
        m_renderPass = new RenderPass(device, AttachmentArray(device, swapchain->GetSwapchainFormat()));
    }

    inline void PipelineShaded::CreateRenderPassResources(Device *device, Swapchain *swapchain, CommandPool *commandPool)
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

    inline void PipelineShaded::CreateRenderPipelineLayout(Device *device)
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		VkDescriptorSetLayout pLayouts = m_descriptorLayout->Get();
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1; // Optional
		pipelineLayoutInfo.pSetLayouts = &pLayouts; // Optional

        // hard coded push constant
		std::vector<VkPushConstantRange> pushConstants;
        PushConstantDef d = PushConstantDef{VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant)};
        pushConstants.push_back(d.Get());

		pipelineLayoutInfo.pPushConstantRanges = pushConstants.data();
		pipelineLayoutInfo.pushConstantRangeCount = pushConstants.size();

		if (vkCreatePipelineLayout(device->Get(), &pipelineLayoutInfo, nullptr, &m_renderPipelineLayout) != VK_SUCCESS)
		{
		    throw std::runtime_error("failed to create pipeline layout!");
		}
    }

    inline void PipelineShaded::CreateRenderPipeline(Device *device, Swapchain *swapchain, CommandPool *commandPool)
    {
        // --=====================================--
        //              SHADER STAGES
        // --=====================================--
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = m_vertexShader->Get();
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = m_fragmentShader->Get();
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        // --=====================================--
        //              DYNAMIC STATE
        // --=====================================--
        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) swapchain->GetSwapchainExtent().width;
        viewport.height = (float) swapchain->GetSwapchainExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapchain->GetSwapchainExtent();

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        // --=====================================--
        //              VERTEX INPUT
        // --=====================================--
        auto bindingDescription = m_vertexBuffer->GetDescription().GetBinding();
        auto attributeDescriptions = m_vertexBuffer->GetDescription().GetDescription();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        // --=====================================--
        //             INPUT ASSEMBLY
        // --=====================================--
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        // --=====================================--
        //                RASERIZER
        // --=====================================--
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer.depthBiasClamp = 0.0f; // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

        // --=====================================--
        //              MULTISAMPLING
        // --=====================================--
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = device->GetPhysicalDeviceProperties().mssaSamples;
        multisampling.minSampleShading = 1.0f; // Optional
        multisampling.pSampleMask = nullptr; // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE; // Optional
        multisampling.sampleShadingEnable = VK_TRUE; // enable sample shading in the pipeline
        multisampling.minSampleShading = .2f; // min fraction for sample shading; closer to one is smoother

        // --=====================================--
        //              COLOR BLEND
        // --=====================================--
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional

        // --=====================================--
        //             DEPTH STENCIL
        // --=====================================--
        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional
 
        // --=====================================--
        //           PIPELINE CREATE INFO
        // --=====================================--
		VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = m_renderPipelineLayout;
        pipelineInfo.renderPass = m_renderPass->Get();
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional

        if (vkCreateGraphicsPipelines(device->Get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_renderPipeline) != VK_SUCCESS)
		{
            KN_WARN("failed to create graphics pipeline!");
        }
    }

    void PipelineShaded::GetDrawCommands(VkCommandBuffer commandBuffer, const RenderFrameData &data)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            assert("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_renderPass->Get();
        renderPassInfo.framebuffer = m_framebuffers.Get(data.swapchainImage)->Get(); // m_swapchain->GetFramebuffer(imageIndex)->Get(); // m_swapChainFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = data.renderExtent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        VkBuffer vertexBuffers[] = {m_vertexBuffer->Get()->Get()};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer->Get()->Get(), 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_renderPipeline);
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(data.renderExtent.width);
        viewport.height = static_cast<float>(data.renderExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = data.renderExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		PushConstant c;
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		c.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		vkCmdPushConstants(commandBuffer, m_renderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &c);
			
        vkCmdBindDescriptorSets(commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS, m_renderPipelineLayout, 0, 1, &m_descriptorSets->Get()[0], 0, nullptr);

        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_indiciesCount), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
            assert("failed to record command buffer!");
        }
    }
}

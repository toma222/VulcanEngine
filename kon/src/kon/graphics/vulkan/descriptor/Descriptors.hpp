
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kon
{
    enum class DescriptorType
    {
        UniformBuffer,
        ImageSampler
    };

    class Descriptor
    {
    public:
        Descriptor() = default;
        virtual ~Descriptor() = default;
        VkDescriptorSetLayoutBinding Get() { return m_binding; }

    protected:
        VkDescriptorSetLayoutBinding m_binding;
    };

    class DescriptorUniformBuffer : public Descriptor
    {
    public:
        DescriptorUniformBuffer();
        ~DescriptorUniformBuffer();
    };
}


#pragma once

#include <kon/graphics/vulkan/pipeline/Attachment.hpp>

// not using this rn

namespace kon
{
    class SubpassDescription
    {
    public:
        SubpassDescription();

        // void AddAttachment(Attachment );

    private:
        VkPipelineBindPoint bindPoint;
        AttachmentColor color;
        AttachmentDepth depth;
        AttachmentColorResolve resolve;

        VkSubpassDependency m_subpassDependency;
        VkSubpassDescription m_subpassDescription;
    };
}

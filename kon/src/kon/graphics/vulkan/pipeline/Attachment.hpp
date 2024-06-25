
#include "vulkan/vulkan_core.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <kon/graphics/vulkan/Device.hpp>
#include <array>

namespace kon
{
    struct AttachmentArray
    {
    public:
        AttachmentArray(Device *device, VkFormat format);

        std::array<VkAttachmentDescription, 3> m_descriptions;

		VkAttachmentReference m_colorRef;
		VkAttachmentReference m_depthRef;
		VkAttachmentReference m_colorResolveRef;
    };
}

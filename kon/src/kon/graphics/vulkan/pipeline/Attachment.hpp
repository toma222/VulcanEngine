
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <kon/graphics/vulkan/Device.hpp>

namespace kon
{
    struct Attachment
    {
    public:
        Attachment(Device *device, VkFormat format, uint32_t index);

        virtual VkAttachmentReference GetReference() = 0;
        virtual VkAttachmentDescription GetDescription() = 0;

    protected:
        Device *m_device  { VK_NULL_HANDLE };
        VkFormat m_format {};
        uint32_t m_index  {0};
    };

    struct AttachmentColor : public Attachment
    {
    public:
        using Attachment::Attachment;

        VkAttachmentReference GetReference() override;
        VkAttachmentDescription GetDescription() override;
    };

    struct AttachmentDepth : public Attachment
    {
    public:
        using Attachment::Attachment;

        VkAttachmentReference GetReference() override;
        VkAttachmentDescription GetDescription() override;
    };

    struct AttachmentColorResolve : public Attachment
    {
    public:
        using Attachment::Attachment;

        VkAttachmentReference GetReference() override;
        VkAttachmentDescription GetDescription() override;
    };

    struct AttachmentArray
    {
    public:
        AttachmentArray(Device *device, VkFormat format);
        void AddColorAttachment(Attachment *attachment);
        void AddDepthAttachment(Attachment *attachment);
        void AddColorResAttachment(Attachment *attachment);

        std::vector<VkAttachmentDescription> m_descriptions;
        VkAttachmentReference m_colorRef;
        VkAttachmentReference m_colorResolveRef;
        VkAttachmentReference m_depthAttachmentRef;
    };
}

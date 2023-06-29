#pragma once

#include <Base/Base.h>

#include <glad/vulkan.h>

namespace aer::gfx::vk
{

struct AttachmentDescription;
struct AttachmentReference;
struct SubpassDescription;

struct RenderPass : public Object, public Interfaces< RenderPass, ICreate >
{
    RenderPass( const AttachmentDescription&, const AttachmentReference&, const SubpassDescription& );

protected:
    VkRenderPass _renderPass;
};

struct AttachmentDescription
{
    VkAttachmentDescriptionFlags    flags           = 0;
    VkFormat                        format          = VK_FORMAT_UNDEFINED;
    VkSampleCountFlagBits           samples         = VK_SAMPLE_COUNT_1_BIT;
    VkAttachmentLoadOp              loadOp          = VK_ATTACHMENT_LOAD_OP_CLEAR;
    VkAttachmentStoreOp             storeOp         = VK_ATTACHMENT_STORE_OP_STORE;
    VkAttachmentLoadOp              stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    VkAttachmentStoreOp             stencilStoreOp  = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    VkImageLayout                   initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageLayout                   finalLayout     = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
};

struct AttachmentReference
{
    uint32_t                        attachment  = 0;
    VkImageLayout                   layout      = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageAspectFlags              aspectMask  = 0;
};
} // namespace aer::gfx::vk

#pragma once

#include <Base/Base.h>

#include <glad/vulkan.h>

#include <vk/LogicalDevice.h>

namespace aer::gfx::vk
{

struct AttachmentDescription;
struct AttachmentReference;
struct SubpassDescription;
struct SubpassDependency;

struct RenderPass : public Object, public Interfaces< RenderPass, ICreate >
{
    using Attachments           = std::vector<AttachmentDescription>;
    using Subpasses             = std::vector<SubpassDescription>;
    using Dependencies          = std::vector<SubpassDependency>;
    using CorrelatedViewMasks   = std::vector<uint32_t>;

    RenderPass
    (
        Device* in_device, const Attachments& in_attachments, const Subpasses& in_subpasses, 
        const Dependencies& in_dependencies, const CorrelatedViewMasks in_correlatedViewMasks = {}
    );
    ~RenderPass();

    const ref_ptr<Device>       device;
    const Attachments           attachments;
    const Subpasses             subpasses;
    const Dependencies          dependencies;
    const CorrelatedViewMasks   correlatedViewMasks;

protected:
    VkRenderPass          _renderPass;
    VkPipelineLayout      _pipelineLayout;
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
    uint32_t                        attachment      = 0;
    VkImageLayout                   layout          = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageAspectFlags              aspectMask      = 0;
};

struct SubpassDescription
{
    VkSubpassDescriptionFlags        flags                   = 0;
    VkPipelineBindPoint              pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    std::vector<AttachmentReference> inputAttachments;              // attachments that are read from shader
    std::vector<AttachmentReference> colorAttachments;              //? attachments used as color buffers?
    std::vector<AttachmentReference> resolveAttachments;            // attachments used for multisampling color attachments
    std::vector<AttachmentReference> depthStencilAttachments;       // attachments for depth & stencil data
    std::vector<uint32_t>            preserveAttachments;           // attachments not used by this subpass, but for which data must be preserved for later subpasses 
    uint32_t                         viewMask               = 0;
    VkResolveModeFlagBits            depthResolveMode       = VK_RESOLVE_MODE_NONE;
    VkResolveModeFlagBits            stencilResolveMode     = VK_RESOLVE_MODE_NONE;
    std::vector<AttachmentReference> depthStencilResolveAttachements;
};

struct SubpassDependency
{
    uint32_t                srcSubpass      = VK_SUBPASS_EXTERNAL;
    uint32_t                dstSubpass      = 0;
    VkPipelineStageFlags    srcStageMask    = 0;
    VkPipelineStageFlags    dstStageMask    = 0;
    VkAccessFlags           srcAccessMask   = 0;
    VkAccessFlags           dstAccessMask   = 0;
    VkDependencyFlags       dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    int32_t                 viewOffset      = 0;
};

} // namespace aer::gfx::vk

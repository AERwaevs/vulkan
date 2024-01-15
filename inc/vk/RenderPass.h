#pragma once

#include "vk.h"

#include <vk/LogicalDevice.h>

namespace aer::gfx::vk
{

struct AttachmentDescription;
struct AttachmentReference;
struct SubpassDescription;
struct SubpassDependency;

struct RenderPass : public Object
{
    using Attachments           = std::vector<AttachmentDescription>;
    using Subpasses             = std::vector<SubpassDescription>;
    using Dependencies          = std::vector<SubpassDependency>;
    using CorrelatedViewMasks   = std::vector<uint32_t>;

    static ref_ptr<RenderPass> create( Device* device, VkFormat imageFormat );
    static ref_ptr<RenderPass> create( Device* device, VkFormat imageFormat, VkFormat depthFormat, bool requiresDepthRead = false );
    static ref_ptr<RenderPass> create( Device* device, VkFormat imageFormat, VkSampleCountFlagBits samples );
    static ref_ptr<RenderPass> create( Device* device, VkFormat imageFormat, VkFormat depthFormat, VkSampleCountFlagBits samples, bool requiresDepthRead = false );
    
    RenderPass( Device*, const Attachments&, const Subpasses&, const Dependencies&, const CorrelatedViewMasks = {} );
    ~RenderPass();
    
    operator VkRenderPass () const { return _renderPass; }

    const ref_ptr<Device>       device;
    const Attachments           attachments;
    const Subpasses             subpasses;
    const Dependencies          dependencies;
    const CorrelatedViewMasks   correlatedViewMasks;

protected:
    VkRenderPass          _renderPass;
};

struct AttachmentDescription
{
    VkAttachmentDescriptionFlags    flags           = 0;
    VkFormat                        format          = VK_FORMAT_UNDEFINED;
    VkSampleCountFlagBits           samples         = VK_SAMPLE_COUNT_1_BIT;
    VkAttachmentLoadOp              loadOp          = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    VkAttachmentStoreOp             storeOp         = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    VkAttachmentLoadOp              stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    VkAttachmentStoreOp             stencilStoreOp  = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    VkImageLayout                   initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageLayout                   finalLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
};

struct AttachmentReference
{
    uint32_t                        attachment      = 0;
    VkImageLayout                   layout          = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageAspectFlags              aspectMask      = VK_IMAGE_ASPECT_NONE;
};

struct SubpassDescription
{
    VkSubpassDescriptionFlags        flags                   = 0;
    VkPipelineBindPoint              pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    std::vector<AttachmentReference> inputAttachments;                // attachments that are read from shader
    std::vector<AttachmentReference> colorAttachments;                // attachments used as color buffers
    std::vector<AttachmentReference> resolveAttachments;              // attachments used for multisampling color attachments
    std::vector<AttachmentReference> depthStencilAttachments;         // attachments for depth & stencil data
    std::vector<uint32_t>            preserveAttachments;             // attachments not used by this subpass, but for which data must be preserved for later subpasses 
    uint32_t                         viewMask               = 0;
    VkResolveModeFlagBits            depthResolveMode       = VK_RESOLVE_MODE_NONE;
    VkResolveModeFlagBits            stencilResolveMode     = VK_RESOLVE_MODE_NONE;
    std::vector<AttachmentReference> depthStencilResolveAttachements; // attachments used for resolving depth & stencil data
};

struct SubpassDependency
{
    uint32_t                srcSubpass      = 0;
    uint32_t                dstSubpass      = 0;
    VkPipelineStageFlags    srcStageMask    = VK_PIPELINE_STAGE_NONE;
    VkPipelineStageFlags    dstStageMask    = VK_PIPELINE_STAGE_NONE;
    VkAccessFlags           srcAccessMask   = VK_ACCESS_NONE;
    VkAccessFlags           dstAccessMask   = VK_ACCESS_NONE;
    VkDependencyFlags       dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    int32_t                 viewOffset      = 0;
};

} // namespace aer::gfx::vk

#include <vk/RenderPass.h>

#include <Base/memory/scratch_memory.h>

namespace aer::gfx::vk
{
    
RenderPass::RenderPass
(
    Device*                   in_device,
    const Attachments&        in_attachments,
    const Subpasses&          in_subpasses,
    const Dependencies&       in_dependencies,
    const CorrelatedViewMasks in_correlatedViewMasks
)
:   device( in_device ),
    attachments( in_attachments ),
    subpasses( in_subpasses ),
    dependencies( in_dependencies ),
    correlatedViewMasks( in_correlatedViewMasks )
{
    auto scratch = scratch_memory::create( 1024 );

    auto copyAttachments = [&]() -> VkAttachmentDescription2*
    {
        if( attachments.empty() ) return VK_NULL_HANDLE;

        auto vk_attachments = scratch->allocate<VkAttachmentDescription2>( attachments.size() );
        for( size_t i = 0; i < attachments.size(); ++i )
        {
            auto& src = attachments[i];
            auto& dst = vk_attachments[i];

            dst.sType           = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2;
            dst.pNext           = VK_NULL_HANDLE;
            dst.flags           = src.flags;
            dst.format          = src.format;
            dst.samples         = src.samples;
            dst.loadOp          = src.loadOp;
            dst.storeOp         = src.storeOp;
            dst.stencilLoadOp   = src.stencilLoadOp;
            dst.stencilStoreOp  = src.stencilStoreOp;
            dst.initialLayout   = src.initialLayout;
            dst.finalLayout     = src.finalLayout;
        }
        return vk_attachments;
    };

    auto copySubpasses = [&]() -> VkSubpassDescription2*
    {
        if( subpasses.empty() ) return VK_NULL_HANDLE;

        auto copyReference = [&]( const std::vector<AttachmentReference>& references ) -> VkAttachmentReference2*
        {
            if( references.empty() ) return VK_NULL_HANDLE;

            auto vk_references = scratch->allocate<VkAttachmentReference2>( references.size() );
            for( size_t i = 0; i < references.size(); ++i )
            {
                auto& src = references[i];
                auto& dst = vk_references[i];

                dst.sType      = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2;
                dst.pNext      = VK_NULL_HANDLE;
                dst.attachment = src.attachment;
                dst.layout     = src.layout;
                dst.aspectMask = src.aspectMask;
            }
            return vk_references;
        };

        auto vk_subpasses = scratch->allocate<VkSubpassDescription2>( subpasses.size() );
        for( size_t i = 0; i < subpasses.size(); ++i )
        {
            auto& src = subpasses[i];
            auto& dst = vk_subpasses[i];

            dst.sType                   = VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2;
            dst.pNext                   = VK_NULL_HANDLE;
            dst.flags                   = src.flags;
            dst.pipelineBindPoint       = src.pipelineBindPoint;
            dst.inputAttachmentCount    = static_cast<uint32_t>( src.inputAttachments.size() );
            dst.pInputAttachments       = copyReference( src.inputAttachments) ;
            dst.colorAttachmentCount    = static_cast<uint32_t>( src.colorAttachments.size() );
            dst.pColorAttachments       = copyReference( src.colorAttachments );
            dst.pResolveAttachments     = copyReference( src.resolveAttachments );
            dst.pDepthStencilAttachment = copyReference( src.depthStencilAttachments );
            dst.preserveAttachmentCount = static_cast<uint32_t>( src.preserveAttachments.size() );
            dst.pPreserveAttachments    = src.preserveAttachments.empty() ? nullptr : src.preserveAttachments.data();
            dst.viewMask                = src.viewMask;
        }
        return vk_subpasses;
    };

    auto copyDependencies = [&]() -> VkSubpassDependency2*
    {
        if( dependencies.empty() ) return VK_NULL_HANDLE;

        auto vk_dependencies = scratch->allocate<VkSubpassDependency2>( dependencies.size() );
        for( size_t i = 0; i < dependencies.size(); ++i )
        {
            auto& src = dependencies[i];
            auto& dst = vk_dependencies[i];

            dst.sType           = VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2;
            dst.pNext           = VK_NULL_HANDLE;
            dst.srcSubpass      = src.srcSubpass;
            dst.dstSubpass      = src.dstSubpass;
            dst.srcStageMask    = src.srcStageMask;
            dst.dstStageMask    = src.dstStageMask;
            dst.srcAccessMask   = src.srcAccessMask;
            dst.dstAccessMask   = src.dstAccessMask;
            dst.dependencyFlags = src.dependencyFlags;
            dst.viewOffset      = src.viewOffset;

        }
        return vk_dependencies;
    };

    VkRenderPassCreateInfo2 createInfo
    {
        VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2,
        VK_NULL_HANDLE,
        VkRenderPassCreateFlags{ 0 },
        static_cast<uint32_t>( attachments.size() ),
        copyAttachments(),
        static_cast<uint32_t>( subpasses.size() ),
        copySubpasses(),
        static_cast<uint32_t>( dependencies.size() ),
        copyDependencies(),
        static_cast<uint32_t>( correlatedViewMasks.size() ),
        correlatedViewMasks.empty() ? VK_NULL_HANDLE : correlatedViewMasks.data(),
    };

    auto result = vkCreateRenderPass2( *device, &createInfo, VK_ALLOCATOR, &_renderPass );
}

RenderPass::~RenderPass()
{
    if( _pipelineLayout ) vkDestroyPipelineLayout( *device, _pipelineLayout, VK_ALLOCATOR );
    if( _renderPass )     vkDestroyRenderPass( *device, _renderPass, VK_ALLOCATOR );
}

ref_ptr<RenderPass> RenderPass::create( Device* device, VkFormat imageFormat, VkFormat depthFormat, bool requiresDepthRead )
{
    RenderPass::Attachments attachments
    {
        { .format = imageFormat }, // colorAttachment
        { .format = depthFormat }  // depthAttachment
    };
    AttachmentReference colorAttachmentRef
    {
        .attachment = 0,
        .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };
    SubpassDependency colorDependency
    {
        .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    };
    AttachmentReference depthAttachmentRef
    {
        .attachment = 1,
        .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };
    SubpassDependency depthDependency
    {
        .srcStageMask  = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
        .dstStageMask  = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
        .srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    };
    RenderPass::Subpasses subpasses
    {{
            .colorAttachments{ colorAttachmentRef },
            .depthStencilAttachments{ depthAttachmentRef }
    }};

    RenderPass::Dependencies dependencies
    {
        colorDependency,
        depthDependency
    };

    return ref_ptr<RenderPass>( new RenderPass( device, attachments, subpasses, dependencies ) );
}

} // namespace aer::gfx::vk

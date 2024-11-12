#include <vk/state/ViewportState.h>

namespace aer::vk
{

ViewportState::ViewportState()
:   viewportState
    {
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            VK_NULL_HANDLE,         // pNext
            VkPipelineViewportStateCreateFlags{ 0 },
            static_cast<uint32_t>( viewports.size() ),
            viewports.empty() ? VK_NULL_HANDLE : viewports.data(),
            static_cast<uint32_t>( scissors.size() ),
            scissors.empty() ? VK_NULL_HANDLE : scissors.data(),
    }
{}

ViewportState::ViewportState( VkExtent2D extent )
:   viewportState
    {
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            VK_NULL_HANDLE,         // pNext
            VkPipelineViewportStateCreateFlags{ 0 },
            static_cast<uint32_t>( viewports.size() ),
            viewports.empty() ? VK_NULL_HANDLE : viewports.data(),
            static_cast<uint32_t>( scissors.size() ),
            scissors.empty() ? VK_NULL_HANDLE : scissors.data(),
    }
{ set( 0u, 0u, extent.width, extent.height ); }

ViewportState::ViewportState( int32_t x, int32_t y, uint32_t width, uint32_t height )
:   viewportState
    {
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            VK_NULL_HANDLE,         // pNext
            VkPipelineViewportStateCreateFlags{ 0 },
            static_cast<uint32_t>( viewports.size() ),
            viewports.empty() ? VK_NULL_HANDLE : viewports.data(),
            static_cast<uint32_t>( scissors.size() ),
            scissors.empty() ? VK_NULL_HANDLE : scissors.data(),
    }
{ set( x, y, width, height ); }

void ViewportState::set( int32_t x, int32_t y, uint32_t width, uint32_t height )
{
    viewports[0] =
    {
        static_cast<float>( x ),
        static_cast<float>( y ),
        static_cast<float>( width ),
        static_cast<float>( height )
    };

    scissors[0] =
    {
        VkOffset2D{ x, y },
        VkExtent2D{ width, height }
    };
}

void ViewportState::apply( VkGraphicsPipelineCreateInfo& pipelineInfo ) const
{    
    pipelineInfo.pViewportState = &viewportState;
}

} // namespace aer::vk

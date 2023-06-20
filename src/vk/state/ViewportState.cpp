#include <vk/state/ViewportState.h>

namespace aer::gfx::vk
{
    
inline const VkViewport defaultViewport
{ 
    0.0f, // x
    0.0f, // y
    0.0f, // width
    0.0f, // height
    0.0f, // minDepth
    1.0f  // maxDepth
};

inline const VkRect2D defaultScissor
{
    { 0u, 0u }, // offset
    { 0u, 0u }  // extent
};

ViewportState::ViewportState()
:   viewports{ defaultViewport }, scissors{ defaultScissor }
{}

ViewportState::ViewportState( VkExtent2D extent )
:   viewports{ defaultViewport }, scissors{ defaultScissor }
{ set( 0u, 0u, extent.width, extent.height ); }

ViewportState::ViewportState( int32_t x, int32_t y, uint32_t width, uint32_t height )
:   viewports{ defaultViewport }, scissors{ defaultScissor }
{ set( x, y, width, height ); }

void ViewportState::set( int32_t x, int32_t y, uint32_t width, uint32_t height )
{
    viewports.resize( 1 );
    scissors.resize( 1 );

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

void ViewportState::apply( VkGraphicsPipelineCreateInfo& pipelineInfo )
{    
    viewportState =
    {
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        VK_NULL_HANDLE,         // pNext
        VkPipelineViewportStateCreateFlags{ 0 },
        static_cast<uint32_t>( viewports.size() ),
        viewports.data(),
        static_cast<uint32_t>( scissors.size() ),
        scissors.data()
    };

    pipelineInfo.pViewportState = &viewportState;
}

} // namespace aer::gfx::vk

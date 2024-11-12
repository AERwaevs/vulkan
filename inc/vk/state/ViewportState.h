#pragma once

#include "GraphicsPipelineState.h"

namespace aer::gfx::vk
{

using Viewports = std::vector<VkViewport>;
using Scissors  = std::vector<VkRect2D>;
    
class ViewportState : public GraphicsPipelineState
{
public:
    Viewports viewports
    {{ 
        0.0f,   // x
        0.0f,   // y
        0.0f,   // width
        0.0f,   // height
        0.0f,   // minDepth
        1.0f    // maxDepth
    }};
    Scissors scissors
    {{
        VkOffset2D{ 0,  0  },
        VkExtent2D{ 0u, 0u }
    }};
public:
                ViewportState();
    explicit    ViewportState( VkExtent2D );
    explicit    ViewportState( int32_t, int32_t, uint32_t, uint32_t );
    virtual     ~ViewportState() = default;
public:
    void        set( int32_t x, int32_t y, uint32_t width, uint32_t height );
    void        apply( VkGraphicsPipelineCreateInfo& ) const override;
private:
    // TODO move to scratch memory allocation
    VkPipelineViewportStateCreateInfo viewportState;
};
    
} // namespace aer::gfx::vk

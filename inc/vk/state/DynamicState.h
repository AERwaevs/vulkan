#pragma once

#include "GraphicsPipelineState.h"

namespace aer::vk
{
    
struct DynamicState : public GraphicsPipelineState
{
    using DynamicStates = std::vector<VkDynamicState>;

                    DynamicState();
    explicit        DynamicState( const DynamicStates& );
    virtual         ~DynamicState() = default;

    DynamicStates   dynamicStates
    {{
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    }};
    
    void            apply( VkGraphicsPipelineCreateInfo& pipelineInfo ) const override;
private:
        // TODO move to scratch memory allocation
    VkPipelineDynamicStateCreateInfo dynamicState;
};
    
} // namespace aer::vk

#pragma once

#include "GraphicsPipelineState.h"

namespace aer::gfx::vk
{
    
struct DynamicState : public GraphicsPipelineState, public Interfaces< DynamicState, ICreate, ITypeInfo >
{
    using DynamicStates = std::vector<VkDynamicState>;

                    DynamicState();
    explicit        DynamicState( const DynamicStates& );

    DynamicStates   dynamicStates
    {{
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    }};
    
    void            apply( VkGraphicsPipelineCreateInfo& pipelineInfo ) const override;
protected:
    virtual         ~DynamicState() = default;
private:
        // TODO move to scratch memory allocation
    VkPipelineDynamicStateCreateInfo dynamicState;
};
    
} // namespace aer::gfx::vk

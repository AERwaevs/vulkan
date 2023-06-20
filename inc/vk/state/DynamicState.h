#pragma once

#include <Base/Base.h>

#include <glad/vulkan.h>

#include "GraphicsPipelineState.h"
#include "ShaderStage.h"

namespace aer::gfx::vk
{
    
class DynamicState : public GraphicsPipelineState, public Interfaces< DynamicState, ICreate, ITypeInfo >
{
public:
    using DynamicStates = std::vector<VkDynamicState>;
    DynamicStates dynamicStates;
public:
    // defaults to { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR }
                DynamicState();
    explicit    DynamicState( const DynamicStates& );
    void        apply( VkGraphicsPipelineCreateInfo& pipelineInfo ) const override;
protected:
    virtual ~DynamicState() = default;
private:
    // TODO move to scratch memory allocation
    VkPipelineDynamicStateCreateInfo dynamicState;
};
    
} // namespace aer::gfx::vk

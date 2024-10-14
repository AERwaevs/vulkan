#pragma once

#include "GraphicsPipelineState.h"

namespace aer::gfx::vk
{
    
struct TessellationState : public inherit< TessellationState, GraphicsPipelineState >
{
    TessellationState( uint32_t in_patchControlPoints = 1);
    virtual         ~TessellationState() = default;

    uint32_t patchControlPoints = 1;

    void        apply( VkGraphicsPipelineCreateInfo& ) const override;
private:
    // TODO move to scratch memory allocation
    VkPipelineTessellationStateCreateInfo tessellationInfo;
};
    
} // namespace aer::gfx::vk

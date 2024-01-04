#pragma once

#include "GraphicsPipelineState.h"

namespace aer::gfx::vk
{
    
struct TessellationState : public GraphicsPipelineState, public Interfaces< TessellationState, ICreate, ITypeInfo >
{
    TessellationState( uint32_t in_patchControlPoints = 1);

    uint32_t patchControlPoints = 1;

    void        apply( VkGraphicsPipelineCreateInfo& ) const override;
protected:
    virtual         ~TessellationState() = default;
private:
    // TODO move to scratch memory allocation
    VkPipelineTessellationStateCreateInfo tessellationInfo;
};
    
} // namespace aer::gfx::vk

#pragma once

#include "GraphicsPipelineState.h"

namespace aer::gfx::vk
{
    
struct VertexInputState : public GraphicsPipelineState
{
    using Bindings   = std::vector<VkVertexInputBindingDescription>;
    using Attributes = std::vector<VkVertexInputAttributeDescription>;

                VertexInputState();
    explicit    VertexInputState( const Bindings&, const Attributes& );
    virtual         ~VertexInputState() = default;

    Bindings    vertexBindingDescriptions;
    Attributes  vertexAttributeDescriptions;

    void        apply( VkGraphicsPipelineCreateInfo& ) const override;
private:
    // TODO move to scratch memory allocation
    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
};
    
} // namespace aer::gfx::vk

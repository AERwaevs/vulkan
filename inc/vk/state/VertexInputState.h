#pragma once

#include <glad/vulkan.h>

#include <Base/Base.h>

#include "GraphicsPipelineState.h"

namespace aer::gfx::vk
{
    
class VertexInputState : public GraphicsPipelineState, public Interfaces< VertexInputState, ICreate, ITypeInfo >
{
public:
    using Bindings   = std::vector<VkVertexInputBindingDescription>;
    using Attributes = std::vector<VkVertexInputAttributeDescription>;
    Bindings    vertexBindingDescriptions;
    Attributes  vertexAttributeDescriptions;
public:
                VertexInputState();
    explicit    VertexInputState( const Bindings&, const Attributes& );
    void        apply( VkGraphicsPipelineCreateInfo& ) const override;
private:
    // TODO move to scratch memory allocation
    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
};
    
} // namespace aer::gfx::vk

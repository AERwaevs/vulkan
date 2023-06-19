#pragma once

#include <glad/vulkan.h>

#include <Base/Base.h>

namespace aer::gfx::vk
{
    
class VertexInputState : public Object, public Interfaces< VertexInputState, ICreate, ITypeInfo >
{
public:
    using Bindings   = std::vector<VkVertexInputBindingDescription>;
    using Attributes = std::vector<VkVertexInputAttributeDescription>;
    
    VertexInputState();
    VertexInputState( const Bindings&, const Attributes& );
    
    Bindings    vertexBindingDescriptions;
    Attributes  vertexAttributeDescriptions;
};
    
} // namespace aer::gfx::vk

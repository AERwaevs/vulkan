#include <vk/state/VertexInputState.h>

namespace aer::gfx::vk
{
    
VertexInputState::VertexInputState()
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo
    {
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        VK_NULL_HANDLE,     // pNext
        VkPipelineVertexInputStateCreateFlags{ 0 },
        0,          // vertexBindingDescriptionCount
        nullptr,    // pVertexBindingDescriptions
        0,          // vertexAttributeDescriptionCount
        nullptr     // pVertexAttributeDescriptions
    };

}
    
VertexInputState::VertexInputState( const Bindings& bindings, const Attributes& attributes )
: vertexBindingDescriptions( bindings ), vertexAttributeDescriptions( attributes )
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo
    {
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        VK_NULL_HANDLE,     // pNext
        VkPipelineVertexInputStateCreateFlags{ 0 },
        static_cast<uint32_t>( vertexBindingDescriptions.size() ),
        vertexBindingDescriptions.data(),
        static_cast<uint32_t>( vertexAttributeDescriptions.size() ),
        vertexAttributeDescriptions.data()
    };
}

} // namespace aer::gfx::vk

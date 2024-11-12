#include <vk/state/VertexInputState.h>

namespace aer::vk
{
    
VertexInputState::VertexInputState()
:   vertexInputInfo
    {
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        VK_NULL_HANDLE,     // pNext
        VkPipelineVertexInputStateCreateFlags{ 0 },
        static_cast<uint32_t>( vertexBindingDescriptions.size() ),
        vertexBindingDescriptions.empty() ? VK_NULL_HANDLE : vertexBindingDescriptions.data(),
        static_cast<uint32_t>( vertexAttributeDescriptions.size() ),
        vertexAttributeDescriptions.empty() ? VK_NULL_HANDLE : vertexAttributeDescriptions.data()
    }
{}
    
VertexInputState::VertexInputState( const Bindings& bindings, const Attributes& attributes )
:   vertexBindingDescriptions( bindings ),
    vertexAttributeDescriptions( attributes ),
    vertexInputInfo
    {
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        VK_NULL_HANDLE,     // pNext
        VkPipelineVertexInputStateCreateFlags{ 0 },
        static_cast<uint32_t>( vertexBindingDescriptions.size() ),
        vertexBindingDescriptions.empty() ? VK_NULL_HANDLE : vertexBindingDescriptions.data(),
        static_cast<uint32_t>( vertexAttributeDescriptions.size() ),
        vertexAttributeDescriptions.empty() ? VK_NULL_HANDLE : vertexAttributeDescriptions.data()
    }
{}

void VertexInputState::apply( VkGraphicsPipelineCreateInfo& pipelineInfo ) const
{
    pipelineInfo.pVertexInputState = &vertexInputInfo;
}

} // namespace aer::vk

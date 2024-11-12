#include <vk/state/DynamicState.h>

namespace aer::vk
{

DynamicState::DynamicState()
:   dynamicState
    {
        VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        VK_NULL_HANDLE,     // pNext
        VkPipelineDynamicStateCreateFlags{ 0 },
        static_cast<uint32_t>( dynamicStates.size() ),
        dynamicStates.empty() ? VK_NULL_HANDLE : dynamicStates.data()
    }
{}

DynamicState::DynamicState( const DynamicStates& states )
:   dynamicStates{ states },
    dynamicState
    {
        VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        VK_NULL_HANDLE,     // pNext
        VkPipelineDynamicStateCreateFlags{ 0 },
        static_cast<uint32_t>( dynamicStates.size() ),
        dynamicStates.empty() ? VK_NULL_HANDLE : dynamicStates.data()
    }
{}

void DynamicState::apply( VkGraphicsPipelineCreateInfo& pipelineInfo ) const
{
    pipelineInfo.pDynamicState = &dynamicState;
}
    
} // namespace aer::vk

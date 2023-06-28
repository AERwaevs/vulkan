#include <vk/state/InputAssemblyState.h>

namespace aer::gfx::vk
{
    
InputAssemblyState::InputAssemblyState()
:   inputAssemblyState
    {
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        VK_NULL_HANDLE,     // pNext
        VkPipelineInputAssemblyStateCreateFlags{ 0 },
        topology,
        primitiveRestartEnable
    }
{}

InputAssemblyState::InputAssemblyState( Topology primitiveTopology, VkBool32 primitiveRestart )
:   topology( primitiveTopology ),
    primitiveRestartEnable( primitiveRestart ),
    inputAssemblyState
    {
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        VK_NULL_HANDLE,     // pNext
        VkPipelineInputAssemblyStateCreateFlags{ 0 },
        topology,
        primitiveRestartEnable
    }
{}

void InputAssemblyState::apply( VkGraphicsPipelineCreateInfo& pipelineInfo ) const
{
    pipelineInfo.pInputAssemblyState = &inputAssemblyState;
}

} // namespace aer::gfx::vk

#include <vk/state/InputAssemblyState.h>

namespace aer::gfx::vk
{
    
InputAssemblyState::InputAssemblyState( VkPrimitiveTopology primitiveTopology, VkBool32 primitiveRestart )
: topology( primitiveTopology ), primitiveRestartEnable( primitiveRestart )
{
    VkPipelineInputAssemblyStateCreateInfo vertexInputInfo
    {
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        VK_NULL_HANDLE,     // pNext
        VkPipelineInputAssemblyStateCreateFlags{ 0 },
        topology,
        primitiveRestartEnable
    };
}

} // namespace aer::gfx::vk

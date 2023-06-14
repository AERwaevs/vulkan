#include "DynamicState.h"

namespace aer::gfx::vk
{
    
DynamicState::DynamicState()
{
    std::vector<VkDynamicState> states
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo createInfo
    {
        VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        VK_NULL_HANDLE,     // pNext
        VkPipelineDynamicStateCreateFlags{ 0 },
        static_cast<uint32_t>( states.size() ),
        states.data()
    };
}
    
} // namespace aer::gfx::vk

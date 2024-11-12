#include <vk/state/DepthStencilState.h>

namespace aer::vk
{

DepthStencilState::DepthStencilState()
:   depthStencilState
    {
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        VkPipelineDepthStencilStateCreateFlags{ 0 },
        depthTestEnable,
        depthWriteEnable,
        depthCompareOp,
        depthBoundsTestEnable,
        stencilTestEnable,
        front,
        back,
        minDepthBounds,
        maxDepthBounds
    }
{}

void DepthStencilState::apply(VkGraphicsPipelineCreateInfo& pipelineCreateInfo) const
{
    pipelineCreateInfo.pDepthStencilState = &depthStencilState;
}

} // namespace aer::vk

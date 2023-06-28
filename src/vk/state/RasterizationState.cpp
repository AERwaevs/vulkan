#include <vk/state/RasterizationState.h>

namespace aer::gfx::vk
{
    
RasterizationState::RasterizationState()
:   rasterizationState
    {
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        VkPipelineRasterizationStateCreateFlags{ 0 },
        depthClampEnable,
        rasterizerDiscardEnable,
        polygonMode,
        cullMode,
        frontFace,
        depthBiasEnable,
        depthBiasConstantFactor,
        depthBiasClamp,
        depthBiasSlopeFactor,
        lineWidth
    }
{}

void RasterizationState::apply( VkGraphicsPipelineCreateInfo& pipelineInfo ) const
{
    pipelineInfo.pRasterizationState = &rasterizationState;
}
    
} // namespace aer::gfx::vk

#include <vk/state/MultisampleState.h>

namespace aer::gfx::vk
{
    
MultisampleState::MultisampleState( VkSampleCountFlagBits samples )
:   rasterizationSamples( samples ),
    multisampleState
    {
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        VkPipelineMultisampleStateCreateFlags{ 0 },
        rasterizationSamples,
        sampleShadingEnable,
        minSampleShading,
        sampleMasks.empty() ? VK_NULL_HANDLE : sampleMasks.data(),
        alphaToCoverageEnable,
        alphaToOneEnable
    }
{}

void MultisampleState::apply(VkGraphicsPipelineCreateInfo& pipelineCreateInfo) const
{
    pipelineCreateInfo.pMultisampleState = &multisampleState;
}

} // namespace aer::gfx::vk
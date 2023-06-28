#pragma once

#include <glad/vulkan.h>

#include <Base/Base.h>

#include "GraphicsPipelineState.h"

namespace aer::gfx::vk
{
    
struct MultisampleState : public GraphicsPipelineState, public Interfaces< MultisampleState, ICreate, ITypeInfo >
{
    using SampleMasks = std::vector<VkSampleMask>;

    MultisampleState( VkSampleCountFlagBits = VK_SAMPLE_COUNT_1_BIT );

    VkSampleCountFlagBits   rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
    VkBool32                sampleShadingEnable   = VK_FALSE;
    float                   minSampleShading      = 0.0f;
    SampleMasks             sampleMasks;
    VkBool32                alphaToCoverageEnable = VK_FALSE;
    VkBool32                alphaToOneEnable      = VK_FALSE;
    
    void        apply( VkGraphicsPipelineCreateInfo& ) const override;
private:
    // TODO move to scratch memory allocation
    VkPipelineMultisampleStateCreateInfo multisampleState;
};
    
} // namespace aer::gfx::vk
#include <vk/state/ColorBlendState.h>

namespace aer::gfx::vk
{
    ColorBlendState::ColorBlendState()
    :   colorBlendState
        {
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            VK_NULL_HANDLE,
            0,
            logicOpEnable,
            logicOp,
            static_cast<uint32_t>( colorBlendAttachments.size() ),
            colorBlendAttachments.empty() ? VK_NULL_HANDLE : colorBlendAttachments.data(),
            { blendConstants[0], blendConstants[1], blendConstants[2], blendConstants[3] }
        }
    {}

    ColorBlendState::ColorBlendState( const Attachments& attachments )
    :   colorBlendAttachments{ attachments },
        colorBlendState
        {
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            VK_NULL_HANDLE,
            0,
            logicOpEnable,
            logicOp,
            static_cast<uint32_t>( attachments.size() ),
            attachments.empty() ? VK_NULL_HANDLE : attachments.data(),
            { blendConstants[0], blendConstants[1], blendConstants[2], blendConstants[3] }
        }
    {}
    
    void ColorBlendState::apply(VkGraphicsPipelineCreateInfo& pipelineCreateInfo) const
    {
        pipelineCreateInfo.pColorBlendState = &colorBlendState;
    }
} // namespace aer::gfx::vk
#pragma once

#include "GraphicsPipelineState.h"

namespace aer::gfx::vk
{
    struct ColorBlendState : public GraphicsPipelineState
    {
        using       Attachments = std::vector<VkPipelineColorBlendAttachmentState>;

                    ColorBlendState();
        explicit    ColorBlendState( const Attachments& );

        VkBool32    logicOpEnable = VK_FALSE;
        VkLogicOp   logicOp       = VK_LOGIC_OP_COPY;
        Attachments colorBlendAttachments
        {{
            VK_FALSE,                                               // blendEnable
            VK_BLEND_FACTOR_ZERO,                                   // srcColorBlendFactor
            VK_BLEND_FACTOR_ZERO,                                   // dstColorBlendFactor
            VK_BLEND_OP_ADD,                                        // colorBlendOp
            VK_BLEND_FACTOR_ZERO,                                   // srcAlphaBlendFactor
            VK_BLEND_FACTOR_ZERO,                                   // dstAlphaBlendFactor
            VK_BLEND_OP_ADD,                                        // alphaBlendOp
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | 
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT     // colorWriteMask
        }};
        float       blendConstants[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

        void apply(VkGraphicsPipelineCreateInfo& pipelineCreateInfo) const override;
    private:
        // TODO move to scratch memory allocation
        VkPipelineColorBlendStateCreateInfo colorBlendState;
    };
} // namespace aer::gfx::vk
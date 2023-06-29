#pragma once

#include <glad/vulkan.h>

#include <Base/Base.h>

#include "GraphicsPipelineState.h"

namespace aer::gfx::vk
{
    struct DepthStencilState : public GraphicsPipelineState, public Interfaces< DepthStencilState, ICreate, ITypeInfo >
    {
        DepthStencilState();
        
        VkBool32         depthTestEnable       = VK_TRUE;
        VkBool32         depthWriteEnable      = VK_TRUE;
        VkCompareOp      depthCompareOp        = VK_COMPARE_OP_GREATER;
        VkBool32         depthBoundsTestEnable = VK_FALSE;
        VkBool32         stencilTestEnable     = VK_FALSE;
        VkStencilOpState front                 = {};
        VkStencilOpState back                  = {};
        float            minDepthBounds        = 0.0f;
        float            maxDepthBounds        = 1.0f;
        
        void    apply( VkGraphicsPipelineCreateInfo& pipelineCreateInfo ) const override;

    private:
        // TODO move to scratch memory allocation
        VkPipelineDepthStencilStateCreateInfo depthStencilState;
    };
} // namespace aer::gfx::vk
#pragma once

#include "GraphicsPipelineState.h"

namespace aer::vk
{
    
struct RasterizationState : public GraphicsPipelineState
{
                        RasterizationState();

    VkBool32            depthClampEnable        = VK_FALSE;
    VkBool32            rasterizerDiscardEnable = VK_FALSE;
    VkPolygonMode       polygonMode             = VK_POLYGON_MODE_FILL;
    VkCullModeFlags     cullMode                = VK_CULL_MODE_BACK_BIT;
    VkFrontFace         frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    VkBool32            depthBiasEnable         = VK_FALSE;
    float               depthBiasConstantFactor = 0.0f;
    float               depthBiasClamp          = 0.0f;
    float               depthBiasSlopeFactor    = 0.0f;
    float               lineWidth               = 1.0f;
    
    void                apply( VkGraphicsPipelineCreateInfo& ) const override;
private:
    // TODO move to scratch memory allocation
    VkPipelineRasterizationStateCreateInfo rasterizationState;
};
    
} // namespace aer::vk

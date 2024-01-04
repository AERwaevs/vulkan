#include <vk/GraphicsPipeline.h>

#include <vk/state/ShaderModule.h>

namespace aer::gfx::vk
{

GraphicsPipeline::GraphicsPipeline( PipelineLayout* in_layout, const ShaderStages& in_shaderStages, const GraphicsPipelineStates& in_pipelineStates, Subpass in_subpass )
:   shaderStages( in_shaderStages ),
    pipelineStates( in_pipelineStates ),
    pipelineLayout( in_layout ),
    subpass( in_subpass )
{

}

void GraphicsPipeline::Compile( Context& context )
{
    if( !_device ) _device = context.device;
    
    auto shaderStageCreateInfo = context.scratchMemory->allocate<VkPipelineShaderStageCreateInfo>( shaderStages.size() );
    for( size_t i = 0; i < shaderStages.size(); i++ )
    {
        shaderStageCreateInfo[i] =
        {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, VK_NULL_HANDLE, VkPipelineShaderStageCreateFlags{ 0 },
            shaderStages[i]->stage,
            *shaderStages[i]->module,
            shaderStages[i]->name.c_str(),
            VK_NULL_HANDLE      // pSpecializationInfo
        };
    };

    VkGraphicsPipelineCreateInfo createInfo
    {
        VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, VK_NULL_HANDLE, VkPipelineCreateFlags{ 0 },
        static_cast<uint32_t>( shaderStages.size() ),
        shaderStageCreateInfo,
        VK_NULL_HANDLE,         // pVertexInputState
        VK_NULL_HANDLE,         // pInputAssemblyState
        VK_NULL_HANDLE,         // pTessellationState
        VK_NULL_HANDLE,         // pViewportState
        VK_NULL_HANDLE,         // pRasterizationState
        VK_NULL_HANDLE,         // pMultisampleState
        VK_NULL_HANDLE,         // pDepthStencilState
        VK_NULL_HANDLE,         // pColorBlendState
        VK_NULL_HANDLE,         // pDynamicState
        *pipelineLayout,
        *context.renderPass,
        subpass,
        VK_NULL_HANDLE,         // basePipelineHandle;
        -1                      // basePipelineIndex;
    };

    for( auto pipelineState : pipelineStates )
    {
        pipelineState->apply( createInfo );
    }

    auto result = vkCreateGraphicsPipelines( *_device, VK_NULL_HANDLE, 1, &createInfo, VK_ALLOCATOR, &_pipeline );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create graphics pipeline: %s", ResultMessage( result ) );
}

GraphicsPipeline::~GraphicsPipeline()
{
    vkDestroyPipeline( *_device, _pipeline, VK_ALLOCATOR );
}

} // namespace aer::gfx::vk

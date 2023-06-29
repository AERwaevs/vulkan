#include <vk/PipelineLayout.h>

namespace aer::gfx::vk
{

PipelineLayout::PipelineLayout( Device* device )
:   _device( device ),
    _pipelineLayout( VK_NULL_HANDLE )
{
    VkPipelineLayoutCreateInfo createInfo
    {
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        VK_NULL_HANDLE,
        VkPipelineCreateFlags{ 0 },
        0,                  // setLayoutCount
        VK_NULL_HANDLE,     // pSetLayouts
        0,                  // pushConstantRangeCount
        VK_NULL_HANDLE,     // pPushConstantRanges
    };

    auto result = vkCreatePipelineLayout( *_device, &createInfo, VK_ALLOCATOR, &_pipelineLayout );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create pipeline layout: %s", ResultMessage( result ) );
}

PipelineLayout::~PipelineLayout()
{
    if( _pipelineLayout != VK_NULL_HANDLE )
    {
        vkDestroyPipelineLayout( *_device, _pipelineLayout, VK_ALLOCATOR );
        _pipelineLayout = VK_NULL_HANDLE;
    }
}

} // namespace aer::gfx::vk
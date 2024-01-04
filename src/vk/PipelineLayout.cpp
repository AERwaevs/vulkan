#include <vk/PipelineLayout.h>

namespace aer::gfx::vk
{

PipelineLayout::PipelineLayout( Device* device, const SetLayouts& in_setLayouts, const PushConstantRanges& in_pushConstantRanges )
:   setLayouts{ in_setLayouts },
    pushConstantRanges{ in_pushConstantRanges },
    _device( device ),
    _pipelineLayout( VK_NULL_HANDLE )
{

}

void PipelineLayout::Compile( Context& context )
{
    if( !_device ) _device = context.device;
    VkPipelineLayoutCreateInfo createInfo
    {
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        VK_NULL_HANDLE,
        VkPipelineCreateFlags{ 0 },
        static_cast<uint32_t>( setLayouts.size() ),
        setLayouts.empty() ? VK_NULL_HANDLE : setLayouts.data(),
        static_cast<uint32_t>( pushConstantRanges.size() ),
        pushConstantRanges.empty() ? VK_NULL_HANDLE : pushConstantRanges.data()
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
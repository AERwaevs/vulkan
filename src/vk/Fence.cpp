#include <vk/Fence.h>

namespace aer::gfx::vk
{
    
Fence::Fence( Device* device, VkFenceCreateFlags flags )
: _device( device )
{
    VkFenceCreateInfo createInfo
    {
        VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, VK_NULL_HANDLE, flags
    };

    auto result = vkCreateFence( *_device, &createInfo, VK_ALLOCATOR, &_fence );
    AE_ERROR_IF( result != VK_SUCCESS, "Failed to create fence: %s", ResultMessage( result ) );
}

Fence::~Fence()
{
    if( _fence ) vkDestroyFence( *_device, _fence, VK_ALLOCATOR );
}

} // namespace aer::gfx::vk

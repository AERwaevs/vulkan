#include <vk/CommandPool.h>

namespace aer::gfx::vk
{

CommandPool::CommandPool( Device* device, uint32_t in_queueFamilyIndex, VkCommandPoolCreateFlags in_flags )
:   queueFamilyIndex( in_queueFamilyIndex ), flags( in_flags ), _device( device )
{
    VkCommandPoolCreateInfo createInfo
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, .pNext = VK_NULL_HANDLE, .flags = flags,
        .queueFamilyIndex = queueFamilyIndex
    };

    auto result = vkCreateCommandPool( *_device, &createInfo, VK_ALLOCATOR, &_commandPool );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create command pool: %s", ResultMessage( result ) );
}

CommandPool::~CommandPool()
{
    if( _commandPool ) vkDestroyCommandPool( *_device, _commandPool, VK_ALLOCATOR );
}

} // namespace aer::gfx::vk
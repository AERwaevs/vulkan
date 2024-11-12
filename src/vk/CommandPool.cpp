#include <vk/CommandPool.h>
#include <vk/CommandBuffer.h>

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

CommandPool::CommandPool( CommandPool&& rhs )
:   queueFamilyIndex( rhs.queueFamilyIndex ), flags( rhs.flags ), _device( std::move( rhs._device ) ), _commandPool( rhs._commandPool )
{
    rhs._commandPool = VK_NULL_HANDLE;
}

CommandPool::~CommandPool()
{
    if( _commandPool ) vkDestroyCommandPool( *_device, _commandPool, VK_ALLOCATOR );
}

void CommandPool::reset( VkCommandPoolResetFlags flags )
{
    auto result = vkResetCommandPool( *_device, _commandPool, flags );
    AE_ERROR_IF( result != VK_SUCCESS, "Failed to reset command pool: %s", ResultMessage( result ) );
}

ref_ptr<CommandBuffer> CommandPool::allocate( VkCommandBufferLevel level )
{
    VkCommandBufferAllocateInfo allocateInfo
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, .pNext = VK_NULL_HANDLE,
        .commandPool = _commandPool, .level = level, .commandBufferCount = 1
    };
    std::scoped_lock lock( _mutex );
    VkCommandBuffer  commandBuffer;

    auto result = vkAllocateCommandBuffers( *_device, &allocateInfo, &commandBuffer );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to allocate command buffer: %s", ResultMessage( result ) );

    return create<CommandBuffer>( this, commandBuffer, level );
}

void CommandPool::free( CommandBuffer* commandBuffer )
{
    if( commandBuffer && commandBuffer->_commandBuffer )
    {
        std::scoped_lock lock( _mutex );
        vkFreeCommandBuffers( *_device, _commandPool, 1, &commandBuffer->_commandBuffer );
    }
}

} // namespace aer::gfx::vk
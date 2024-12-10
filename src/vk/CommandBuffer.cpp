#include <vk/CommandBuffer.h>

namespace aer::vk
{

CommandBuffer::CommandBuffer( CommandPool* commandPool, VkCommandBuffer commandBuffer, VkCommandBufferLevel level )
:   _commandPool( commandPool ), _commandBuffer( commandBuffer ), _level( level )
{

}

CommandBuffer::~CommandBuffer()
{
    if( _commandBuffer ) _commandPool->free( this );
}

void CommandBuffer::begin( VkCommandBufferUsageFlags flags )
{
    VkCommandBufferBeginInfo beginInfo
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .pNext = VK_NULL_HANDLE, .flags = flags,
        .pInheritanceInfo = VK_NULL_HANDLE
    };

    auto result = vkBeginCommandBuffer( _commandBuffer, &beginInfo );
    CHECK_F( result == VK_SUCCESS, "Failed to begin command buffer: %s", ResultMessage( result ) );
}

void CommandBuffer::reset( VkCommandBufferResetFlags flags )
{
    auto result = vkResetCommandBuffer( _commandBuffer, flags );
    CHECK_F( result == VK_SUCCESS, "Failed to reset command buffer: %s", ResultMessage( result ) );

    _commandPool->reset();
}

} // namespace aer::vk
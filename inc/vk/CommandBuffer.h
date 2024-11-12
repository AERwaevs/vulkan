#pragma once

#include <vk/vk.h>

#include <vk/CommandPool.h>

namespace aer::gfx::vk
{

class CommandBuffer : public Object
{
public:
    CommandBuffer( CommandPool*, VkCommandBuffer, VkCommandBufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY );
    virtual ~CommandBuffer();

    operator VkCommandBuffer() const { return _commandBuffer; }

    void begin( VkCommandBufferUsageFlags = VkCommandBufferUsageFlags{ 0 } );

protected:
    void reset( VkCommandBufferResetFlags = VkCommandBufferUsageFlags{ 0 } );

    friend CommandPool;
private:
    ref_ptr<CommandPool>    _commandPool;
    VkCommandBuffer         _commandBuffer;
    VkCommandBufferLevel    _level;
};


} // namespace aer::gfx::vk
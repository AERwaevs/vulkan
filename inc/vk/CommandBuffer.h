#pragma once

#include <vk/vk.h>

#include <vk/CommandPool.h>

namespace aer::gfx::vk
{

class CommandBuffer : public inherit< CommandBuffer, Object >
{
public:
    CommandBuffer( CommandPool*, VkCommandBuffer, VkCommandBufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY );

    operator VkCommandBuffer() const { return _commandBuffer; }

    void begin( VkCommandBufferUsageFlags );

protected:
    void reset( VkCommandBufferResetFlags = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT );

    virtual ~CommandBuffer();
    friend CommandPool;
private:
    ref_ptr<CommandPool>    _commandPool;
    VkCommandBuffer         _commandBuffer;
    VkCommandBufferLevel    _level;
};


} // namespace aer::gfx::vk
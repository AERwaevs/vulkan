#pragma once

#include <vk/vk.h>
#include <vk/LogicalDevice.h>

namespace aer::gfx::vk
{

class CommandBuffer;

class CommandPool : public Object, public Interfaces< CommandPool, ICreate >
{
public:
    CommandPool( Device* device, uint32_t in_queueFamilyIndex, VkCommandPoolCreateFlags in_flags = 0 );

    operator VkCommandPool() const { return _commandPool; }

    const uint32_t                  queueFamilyIndex;
    const VkCommandPoolCreateFlags  flags;

    void reset( VkCommandPoolResetFlags = VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT );

    ref_ptr<CommandBuffer> allocate( VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY );
protected:
    void free( CommandBuffer* commandBuffer );

    friend CommandBuffer;
    virtual ~CommandPool();

private:
    std::mutex      _mutex;
    ref_ptr<Device> _device;
    VkCommandPool   _commandPool;
};
    
} // namespace aer::gfx::

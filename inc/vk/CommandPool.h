#pragma once

#include <vk/vk.h>
#include <vk/LogicalDevice.h>

#include <mutex>

namespace aer::vk
{

class CommandBuffer;

class CommandPool : public Object
{
public:
    CommandPool( ref_ptr<Device> device, uint32_t in_queueFamilyIndex, VkCommandPoolCreateFlags in_flags = 0 );
    CommandPool( const CommandPool& ) = delete;
    CommandPool( CommandPool&& rhs );
    virtual ~CommandPool();
    operator VkCommandPool() const { return _commandPool; }

    const uint32_t                  queueFamilyIndex;
    const VkCommandPoolCreateFlags  flags;

    void reset( VkCommandPoolResetFlags = VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT );

    ref_ptr<CommandBuffer> allocate( VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY );
protected:
    void free( CommandBuffer* commandBuffer );

    friend CommandBuffer;

private:
    std::mutex      _mutex;
    ref_ptr<Device> _device;
    VkCommandPool   _commandPool;
};
    
} // namespace aer::

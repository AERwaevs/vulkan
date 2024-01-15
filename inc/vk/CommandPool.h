#pragma once

#include <vk/vk.h>
#include <vk/LogicalDevice.h>

namespace aer::gfx::vk
{

class CommandPool : public Object, public Interfaces< CommandPool, ICreate >
{
public:
    CommandPool( Device* device, uint32_t in_queueFamilyIndex, VkCommandPoolCreateFlags in_flags = 0 );

    operator VkCommandPool() const { return _commandPool; }

    const uint32_t                  queueFamilyIndex;
    const VkCommandPoolCreateFlags  flags;

protected:
    virtual ~CommandPool();

private:
    ref_ptr<Device> _device;
    VkCommandPool   _commandPool;
};
    
} // namespace aer::gfx::

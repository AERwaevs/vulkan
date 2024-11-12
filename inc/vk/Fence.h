#pragma once

#include <vk/LogicalDevice.h>
#include <vk/Semaphore.h>

namespace aer::vk
{
    
class Fence : public inherit< Fence, Object >
{
public:
    explicit            Fence( Device* device, VkFenceCreateFlags flags = 0 );
    virtual             ~Fence();
    operator VkFence()                           const { return _fence; }
public:
            VkFence     vk()                     const { return _fence; }

            Device*     device()                       { return _device.get(); }
    const   Device*     device()                 const { return _device.get(); }

            Semaphores& dependantSemaphores()          { return _dependantSemaphores; }
    const   Semaphores& dependantSemaphores()    const { return _dependantSemaphores; }

    inline  VkResult    wait( uint64_t timeout ) const { return vkWaitForFences( *_device, 1, &_fence, VK_TRUE, timeout ); };
    inline  VkResult    reset()                  const { return vkResetFences( *_device, 1, &_fence ); };
    inline  VkResult    status()                 const { return vkGetFenceStatus( *_device, _fence ); };

protected:
    VkFence             _fence;
    ref_ptr<Device>     _device;

    Semaphores          _dependantSemaphores;
};

} // namespace aer::vk
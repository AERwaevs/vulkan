#pragma once

#include <vk/LogicalDevice.h>

namespace aer::vk
{
    
class Semaphore : public Object
{
public:
    explicit Semaphore( Device* device, VkPipelineStageFlags flags = 0, void* pNextCreateInfo = nullptr );
    virtual ~Semaphore();
public:
            operator VkSemaphore()         const { return _semaphore; }
                     VkSemaphore  vk()     const { return _semaphore; }

            Device*               device()       { return _device.get(); }
    const   Device*               device() const { return _device.get(); }

            VkPipelineStageFlags& flags()        { return _flags; }
    const   VkPipelineStageFlags& flags()  const { return _flags; }
protected:
    VkSemaphore          _semaphore;
    ref_ptr<Device>      _device;
    VkPipelineStageFlags _flags = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
};

using Semaphores = std::vector<ref_ptr<Semaphore>>;

} // namespace aer::

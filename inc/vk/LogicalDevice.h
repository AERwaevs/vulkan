#pragma once

#include "vk.h"
#include "Instance.h"
#include "PhysicalDevice.h"

namespace aer::vk
{

struct QueueSetting
{
    int             queue_family_index{ -1 };
    Vector<float>   queue_priorities{};
};
using QueueSettings = Vector<QueueSetting>;

class Device : public Object
{
public:
    Device( ref_ptr<PhysicalDevice> physical_device, ref_ptr<Surface> surface,
            const QueueSettings& queue_settings );
    operator    VkDevice() const { return _device; }
    ~Device() noexcept;
    
    template< typename F >
    VkResult GetProcAddr( F& proc_addr, const char* name ) const
    {
        proc_addr = reinterpret_cast<F>( vkGetDeviceProcAddr( _device, name ) );
        if( proc_addr == nullptr )
        {
            AE_WARN( "Failed to get procedural address for %s: vk%d", 
                        name,
                        VK_ERROR_EXTENSION_NOT_PRESENT );
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
        else return VK_SUCCESS;
    }

public:
    static inline const Names RequiredLayers
    {
#ifndef NDEBUG
        "VK_LAYER_KHRONOS_validation"
#endif 
    };
    
    static inline const Names RequiredExtensions
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };


private:
    VkDevice                _device;

    ref_ptr<Instance>       _instance;
    ref_ptr<PhysicalDevice> _physical_device;

    VkQueue                 _queue_graphics;
    VkQueue                 _queue_present;
};

} // namespace aer::vk
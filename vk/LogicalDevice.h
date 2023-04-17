#pragma once

#include <glad/vulkan.h>

#include "Instance.h"
#include "PhysicalDevice.h"

namespace AEON::Graphics::vk
{

class Device : public virtual Object, public Implements< Device, ICreate >
{
public:
                Device( Shared<PhysicalDevice> physical_device, Shared<Surface> surface );
    operator    VkDevice() const { return _device; }

    
    template< typename F >
    VkResult GetProcAddr( F& proc_addr, const char* name ) const
    {
        proc_addr = reinterpret_cast<F>( vkGetDeviceProcAddr( _instance, name ) );
        if( proc_addr == nullptr )
        {
            AE_WARN( "Failed to get procedural address for %s: vk%d", 
                        name,
                        VK_ERROR_EXTENSION_NOT_PRESENT );
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
        else return VK_SUCCESS;
    }

protected:
    virtual ~Device();

private:
    VkDevice                _device;

    Shared<Instance>        _instance;
    Shared<PhysicalDevice>  _physical_device;

    VkQueue                 _queue_graphics;
    VkQueue                 _queue_present;

public:
    static const Names RequiredLayers() { return
    {
#ifdef  AEON_DEBUG
        "VK_LAYER_KHRONOS_validation"
#endif 
    }; }
    
    static const Names RequiredExtensions() { return
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    }; }
};

}
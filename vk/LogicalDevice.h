#pragma once

#include <glad/vulkan.h>

#include "Instance.h"
#include "PhysicalDevice.h"

namespace AEON::Graphics::vk
{

class Device : public Object< Device >
{
public:
                Device( Shared<PhysicalDevice> physical_device, Shared<Surface> surface );
    operator    VkDevice() const { return m_device; }

    
    template< typename F >
    VkResult GetProcAddr( F& proc_addr, const char* name ) const
    {
        proc_addr = reinterpret_cast<F>( vkGetDeviceProcAddr( m_instance, name ) );
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
    VkDevice                m_device;

    Shared<Instance>        m_instance;
    Shared<PhysicalDevice>  m_physical_device;

    VkQueue                 m_queue_graphics;
    VkQueue                 m_queue_present;

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
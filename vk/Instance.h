#pragma once

#include <glad/vulkan.h>

#include "Layers.h"
#include "Extensions.h"

//* Define to later control vulkan memory management
#define VK_ALLOCATOR VK_NULL_HANDLE

namespace AEON::Graphics::vk
{
    class PhysicalDevice;

    class Instance : public Singleton< Instance >
    {
    public:
                    Instance( Names instance_extensions = GetRequiredInstanceExtensions(),
                              Names layers = GetRequiredLayers() );
        operator    VkInstance() const { return m_instance; }

        using PhysicalDevices = std::vector<ref_ptr<PhysicalDevice>>;
        PhysicalDevices& physical_devices() { return m_physical_devices; }

        template< typename F >
        VkResult GetProcAddr( F& proc_addr, const char* name ) const
        {
            proc_addr = reinterpret_cast<F>( vkGetInstanceProcAddr( m_instance, name ) );
            if( proc_addr == VK_NULL_HANDLE )
            {
                AE_WARN( "Failed to get procedural address for %s: vk%d", 
                         name,
                         VK_ERROR_EXTENSION_NOT_PRESENT );
                return VK_ERROR_EXTENSION_NOT_PRESENT;
            }
            else return VK_SUCCESS;
        }

    protected:
        virtual ~Instance();

    private:

        VkInstance                      m_instance;
        PhysicalDevices                 m_physical_devices;

#ifdef AEON_DEBUG
        VkDebugUtilsMessengerEXT            m_debug_messenger;
        PFN_vkCreateDebugUtilsMessengerEXT  CreateDebugUtilsMessenger   = VK_NULL_HANDLE;
        PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessenger  = VK_NULL_HANDLE;
#endif
    };
}
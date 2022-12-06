#pragma once

#include <Base/Base.h>

#include <Vendor/glad/include/glad/vulkan.h>

namespace AEON::Graphics::vk
{
    class AEON_DLL Instance : public Inherit< Object, Instance >
    {
    public:
        using PhysicalDevices = Vector<VkPhysicalDevice>;

        Instance( Vector<const char*> instance_extensions, 
                  Vector<const char*> layers );

        template< typename F >
        VkResult GetProcAddr( F& proc_addr, const char* name ) const
        {
            proc_addr = reinterpret_cast<F>( vkGetInstanceProcAddr( m_instance, name ) );
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
        virtual ~Instance();

    private:

        VkInstance                      m_instance;
        VkDebugUtilsMessengerEXT        m_vk_debug_messenger;
        VkPhysicalDevice                m_physical_device;
        VkDevice                        m_device;
        VkQueue                         m_queue_graphics;
        VkSurfaceKHR                    m_surface;
        VkQueue                         m_queue_present;

        PFN_vkCreateDebugUtilsMessengerEXT  _vkCreateDebugUtilsMessengerEXT  = nullptr;
        PFN_vkDestroyDebugUtilsMessengerEXT _vkDestroyDebugUtilsMessengerEXT = nullptr;
    };
}
#pragma once

#include "vk.h"

namespace aer::gfx::vk
{
    InstanceLayerProperties     EnumerateInstanceLayerProperties();
    InstanceExtensionProperties EnumerateInstanceExtensionProperties( Name layer_name );
    Names                       ValidateInstanceLayerNames( Names& names );
    std::string                 UnpackNames( const Names& names );

    struct PhysicalDevice;
    struct Surface;

    struct Instance : public Object, public Interfaces< Instance, ICreate, ISingleton >
    {
    public:
        Instance( Names extensions = RequiredExtensions, Names layers = RequiredLayers );

        operator    VkInstance() const { return _instance; }

        using PhysicalDevices = std::vector<ref_ptr<PhysicalDevice>>;
        PhysicalDevices&         physical_devices() { return _physical_devices; }
        ref_ptr<PhysicalDevice>  physical_device( VkQueueFlags flags, Surface* surface, const PhysicalDeviceTypes& prefs = DevicePreferences );

        template< typename F >
        VkResult GetProcAddr( F& proc_addr, const char* name ) const
        {
            proc_addr = reinterpret_cast<F>( vkGetInstanceProcAddr( _instance, name ) );
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

        VkInstance      _instance;
        PhysicalDevices _physical_devices;
        
    private:

#ifndef NDEBUG
        VkDebugUtilsMessengerEXT            _debug_messenger;
        PFN_vkCreateDebugUtilsMessengerEXT  CreateDebugUtilsMessenger   = VK_NULL_HANDLE;
        PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessenger  = VK_NULL_HANDLE;
#endif
    public:
        static inline const PhysicalDeviceTypes DevicePreferences
        {
            VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
            VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
            VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU
        };
        static inline const Names RequiredLayers
        {
#ifndef NDEBUG
            "VK_LAYER_KHRONOS_validation"
#endif 
        };
    protected:
        static inline const Names RequiredExtensions
        {
            VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef  AEON_PLATFORM_WINDOWS
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif   AEON_PLATFORM_IOS
            VK_EXT_METAL_SURFACE_EXTENSION_NAME,
#elif   AEON_PLATFORM_MACOS
            "VK_MVK_macos_surface",
#elif   AEON_PLATFORM_ANDROID
            "VK_KHR_android_surface",
#elif   AEON_PLATFORM_LINUX
            VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#endif
#ifndef NDEBUG
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
        };
    };
}
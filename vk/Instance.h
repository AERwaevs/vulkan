#pragma once

#include <Base/Base.h>

#include <glad/vulkan.h>

//* Define to later control vulkan memory management
#define VK_ALLOCATOR VK_NULL_HANDLE

namespace AEON::Graphics::vk
{
    using Name                          = const char*;
    using Names                         = std::vector<Name>;
    using InstanceLayerProperties       = std::vector<VkLayerProperties>;
    using InstanceExtensionProperties   = std::vector<VkExtensionProperties>;

    InstanceLayerProperties     EnumerateInstanceLayerProperties();
    InstanceExtensionProperties EnumerateInstanceExtensionProperties( Name layer_name );
    Names                       ValidateInstanceLayerNames( Names& names );

    class PhysicalDevice;

    class Instance : public Object, public Implements< Instance, ICreatable, ISingleton >
    {
    public:
        Instance( Names extensions = RequiredExtensions(), Names layers = RequiredLayers() );

        operator    VkInstance() const { return _instance; }

        using PhysicalDevices = std::vector<ref_ptr<PhysicalDevice>>;
        PhysicalDevices& physical_devices() { return _physical_devices; }

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

#ifdef AEON_DEBUG
        VkDebugUtilsMessengerEXT            _debug_messenger;
        PFN_vkCreateDebugUtilsMessengerEXT  CreateDebugUtilsMessenger   = VK_NULL_HANDLE;
        PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessenger  = VK_NULL_HANDLE;
#endif
    public:
        static const Names RequiredLayers() { return
        {
#ifdef  AEON_DEBUG
            "VK_LAYER_KHRONOS_validation"
#endif 
        }; }
    protected:
        static const Names RequiredExtensions() { return
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
#ifdef  AEON_DEBUG
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
        }; }
    };
}
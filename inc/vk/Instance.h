#pragma once

#include <Base/Base.h>

#include <glad/vulkan.h>

//* Define to later control vulkan memory management
#define VK_ALLOCATOR VK_NULL_HANDLE

namespace aer::gfx::vk
{
    using Name                          = const char*;
    using Names                         = std::vector<Name>;
    using PhysicalDeviceTypes           = std::vector<VkPhysicalDeviceType>;
    using InstanceLayerProperties       = std::vector<VkLayerProperties>;
    using InstanceExtensionProperties   = std::vector<VkExtensionProperties>;

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
        ref_ptr<PhysicalDevice>  physical_device( VkQueueFlags flags, Surface* surface, const PhysicalDeviceTypes& prefs );

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
        static inline const PhysicalDeviceTypes DevicePreferences
        {
            VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
            VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
            VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU
        };
        static inline const Names RequiredLayers
        {
#ifdef  AEON_DEBUG
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
#ifdef  AEON_DEBUG
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
        };
    };

#ifdef  AEON_DEBUG
    constexpr const bool enable_ctor_logging = false;
    constexpr const bool enable_dtor_logging = false;
#endif

#define VK_INFO_CTOR( type ) AE_INFO_IF( enable_ctor_logging, "Creating "#type )
#define VK_INFO_DTOR( type ) AE_INFO_IF( enable_dtor_logging, "Destroying "#type )

    constexpr const char* ResultMessage( VkResult result )
    {
        switch( result )
        {
            case VK_SUCCESS:                              return "VK_SUCCESS";
            case VK_NOT_READY:                            return "VK_NOT_READY";
            case VK_TIMEOUT:                              return "VK_TIMEOUT";
            case VK_EVENT_SET:                            return "VK_EVENT_SET";
            case VK_EVENT_RESET:                          return "VK_EVENT_RESET";
            case VK_INCOMPLETE:                           return "VK_INCOMPLETE";
            case VK_ERROR_OUT_OF_HOST_MEMORY:             return "VK_ERROR_OUT_OF_HOST_MEMORY";
            case VK_ERROR_OUT_OF_DEVICE_MEMORY:           return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
            case VK_ERROR_INITIALIZATION_FAILED:          return "VK_ERROR_INITIALIZATION_FAILED";
            case VK_ERROR_DEVICE_LOST:                    return "VK_ERROR_DEVICE_LOST";
            case VK_ERROR_MEMORY_MAP_FAILED:              return "VK_ERROR_MEMORY_MAP_FAILED";
            case VK_ERROR_LAYER_NOT_PRESENT:              return "VK_ERROR_LAYER_NOT_PRESENT";
            case VK_ERROR_EXTENSION_NOT_PRESENT:          return "VK_ERROR_EXTENSION_NOT_PRESENT";
            case VK_ERROR_FEATURE_NOT_PRESENT:            return "VK_ERROR_FEATURE_NOT_PRESENT";
            case VK_ERROR_INCOMPATIBLE_DRIVER:            return "VK_ERROR_INCOMPATIBLE_DRIVER";
            case VK_ERROR_TOO_MANY_OBJECTS:               return "VK_ERROR_TOO_MANY_OBJECTS";
            case VK_ERROR_FORMAT_NOT_SUPPORTED:           return "VK_ERROR_FORMAT_NOT_SUPPORTED";
            case VK_ERROR_FRAGMENTED_POOL:                return "VK_ERROR_FRAGMENTED_POOL";
            case VK_ERROR_UNKNOWN:                        return "VK_ERROR_UNKNOWN";
            case VK_ERROR_OUT_OF_POOL_MEMORY:             return "VK_ERROR_OUT_OF_POOL_MEMORY";
            case VK_ERROR_INVALID_EXTERNAL_HANDLE:        return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
            case VK_ERROR_FRAGMENTATION:                  return "VK_ERROR_FRAGMENTATION";
            case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
            case VK_ERROR_SURFACE_LOST_KHR:               return "VK_ERROR_SURFACE_LOST_KHR";
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:       return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
            case VK_SUBOPTIMAL_KHR:                       return "VK_SUBOPTIMAL_KHR";
            case VK_ERROR_OUT_OF_DATE_KHR:                return "VK_ERROR_OUT_OF_DATE_KHR";
            case VK_RESULT_MAX_ENUM:                      return "VK_RESULT_MAX_ENUM";
            default:                                      return "UNKNOWN VULKAN ERROR";
        }
    }
}
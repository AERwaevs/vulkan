#pragma once

#if !defined( AEON_PLATFORM_WINDOWS )
    #include <vulkan/vulkan.h>

    inline bool vk_supported() { return true; }
#else
    #define VK_USE_PLATFORM_WIN32_KHR
    #include <glad/vulkan.h>

    #define AEON_VK_VERSION GLAD_MAKE_VERSION( 1, 2 )

    inline bool vk_supported()
    {
        static bool loaded( false );
        if( loaded ) return true;
        static auto version( gladLoaderLoadVulkan( nullptr, nullptr, nullptr ) );
        return loaded = ( version >= AEON_VK_VERSION );
    }
#endif

#include <Base/Base.h>

//* Define to later control vulkan memory management
#define VK_ALLOCATOR VK_NULL_HANDLE

namespace aer::gfx::vk
{
    template< typename T >
    using Vector                        = std::vector<T>;
    using Name                          = const char*;
    using Names                         = Vector<Name>;

    constexpr inline auto ResultMessage( VkResult result )
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
        
    #ifndef NDEBUG
    VKAPI_ATTR static inline VkBool32 debug_callback
    (
        VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
        VkDebugUtilsMessageTypeFlagsEXT             type,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void*                                       pUserData
    )
    {
        switch ( severity )
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            {
                AE_INFO_RAW( "%s", pCallbackData->pMessage );
                break;
            }
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            {
                AE_WARN_RAW( "%s", pCallbackData->pMessage );
                break;
            }
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            {
                AE_ERROR_RAW( "%s", pCallbackData->pMessage );
                break;
            }        
            default:
                break;
        }

        
        AE_INFO_IF( type == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                    "VK_PERFORMANCE %s", pCallbackData->pMessage );
        return VK_FALSE; // only used if testing validation layers
    };
    #endif

}
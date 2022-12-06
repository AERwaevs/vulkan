#pragma once

#include <Base/Base.h>

#include <Vendor/glad/include/glad/vulkan.h>

namespace AEON::Graphics::vk
{
    inline Vector<VkExtensionProperties> EnumerateInstanceExtensionProperties( 
        const char* layer_name = nullptr )
    {
        VkResult result( VK_SUCCESS );
        uint32_t vk_extension_count( 0 );
        result = vkEnumerateInstanceExtensionProperties( layer_name, 
                                                         &vk_extension_count, 
                                                         nullptr );
        AE_ERROR_IF( result != VK_SUCCESS, 
                     "Failed to enumerate instance extension count: vk%d", result );

        Vector<VkExtensionProperties> vk_extensions(vk_extension_count);
        result = vkEnumerateInstanceExtensionProperties( layer_name, 
                                                         &vk_extension_count, 
                                                         vk_extensions.data() );
        AE_ERROR_IF( result != VK_SUCCESS, 
                     "Failed to enumerate instance extension properties: vk%d", result );

        return vk_extensions;
    }

    inline bool ExtensionSupported( const char* extension_name )
    {
        return true;
    }

    inline Vector<const char*> GetRequiredExtensions()
    {
        VkResult result( VK_SUCCESS );

        Vector<const char*> required_extensions( {
            VK_KHR_SURFACE_EXTENSION_NAME,
// surfaces
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

#ifndef AEON_RELEASE
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
        } );

        return required_extensions;
    }
}
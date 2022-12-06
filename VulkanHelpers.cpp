#include "VulkanHelpers.h"

namespace AEON::Graphics::vk
{
    VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback
    ( 
        VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
        VkDebugUtilsMessageTypeFlagsEXT             type,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void*                                       pUserData
    )
    {   
        switch ( severity )
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            {
                AE_WARN( "Vulkan %s", pCallbackData->pMessage );
                break;
            }
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            {
                AE_ERROR( "Vulkan %s", pCallbackData->pMessage );
                break;
            }        
            default:
                break;
        }         
        
        AE_INFO_IF( type == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                    "VK_PERFORMANCE %s", pCallbackData->pMessage );
        return VK_FALSE; // only used if testing validation layers
    }

    bool device_supported( VkPhysicalDevice device )
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties( device, &properties );

        bool   supported( false );
               supported |= properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
               supported |= properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
               supported |= find_queue_families( device ).complete();
        return supported;
    }

    int device_capability( VkPhysicalDevice device )
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties( device, &properties );

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures( device, &features );

        int    score( device_supported( device ) );
               score += properties.limits.maxImageDimension2D;
               score += features.geometryShader;
               score += features.shaderFloat64;
               score += features.largePoints;
               score += features.multiDrawIndirect;
               score += features.variableMultisampleRate;
        return score;
    }

    VkPhysicalDevice pick_device( VkInstance instance )
    {
        uint32_t dev_count = 0;
        vkEnumeratePhysicalDevices( instance, &dev_count, nullptr );
        AE_FATAL_IF( dev_count == 0, "Failed to find a GPU with Vulkan support" );

        Vector<VkPhysicalDevice> devices( dev_count );
        vkEnumeratePhysicalDevices( instance, &dev_count, devices.data() );

        std::multimap<int, VkPhysicalDevice> candidates;
        for( const auto& device : devices )
        {
            candidates.insert( std::make_pair( device_capability( device ), device ) );
        }

        return candidates.rbegin()->first > 0 ? candidates.rbegin()->second : nullptr;
    }

    QueueFamilyIndices find_queue_families
    ( 
        VkPhysicalDevice device, 
        VkSurfaceKHR surface
    )
    {
        QueueFamilyIndices indices;

        uint32_t queue_count( 0 );
        vkGetPhysicalDeviceQueueFamilyProperties( device, &queue_count, nullptr );

        Vector<VkQueueFamilyProperties> queue_families( queue_count );
        vkGetPhysicalDeviceQueueFamilyProperties( device, &queue_count, queue_families.data() );

        auto graphics_support = [&]( int i )
        {
            return queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT;
        };

        auto present_support = [&]( int i )
        { 
            VkBool32 ret = VK_FALSE;
            if( surface == VK_NULL_HANDLE ) return ret;
            vkGetPhysicalDeviceSurfaceSupportKHR( device, i, surface, &ret );
            return ret;
        };

        for( int i = 0; i < queue_count; i++ )
        {
            if( graphics_support( i ) ) indices.graphics_family = i;
            if( present_support( i ) )  indices.present_family  = i;
            if( indices.complete() ) break;
        }

        return indices;
    }
}
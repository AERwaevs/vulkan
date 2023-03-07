#include "Swapchain.h"


namespace AEON::Graphics::vk
{

SwapChainSupportDetails QuerySwapchainSupport( VkPhysicalDevice device, VkSurfaceKHR surface )
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR( device, surface, &details.capabilities );

    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR( device, surface, &format_count, VK_NULL_HANDLE );

    if( format_count > 0 )
    {
        details.formats.resize( format_count );
        vkGetPhysicalDeviceSurfaceFormatsKHR( device, surface, &format_count, details.formats.data() );
    }

    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR( device, surface, &present_mode_count, VK_NULL_HANDLE );

    if( present_mode_count > 0 )
    {
        details.present_modes.resize( present_mode_count );
        vkGetPhysicalDeviceSurfacePresentModesKHR( device, surface, &present_mode_count, details.present_modes.data() );
    }

    return details;
}

}// namespace AEON::Graphics::vk
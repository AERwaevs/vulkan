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

VkSurfaceFormatKHR SelectSwapSurfaceFormat( const SwapChainSupportDetails& details, VkSurfaceFormatKHR preferred )
{
    const auto  default_format      = VK_FORMAT_B8G8R8A8_UNORM;
    const auto  default_colorspace  = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

    if( details.formats.empty() || ( details.formats.size() == 1 && details.formats[0].format == VK_FORMAT_UNDEFINED ) )
    {
        AE_WARN( "Swapchain format undefined, using fallback" ); return { default_format, default_colorspace };
    }

    for( const auto& available : details.formats )
    {
        if( available.format == preferred.format && available.colorSpace == preferred.colorSpace ) return available;
    }

    AE_WARN( "Preferred swapchain format unavailable, trying default" );
    for( const auto& available : details.formats )
    {
        if( available.format == default_format && available.colorSpace == default_colorspace ) return available;
    }

    AE_WARN( "Default swapchain format unavailable, using first available" );
    return details.formats[0];
}

VkPresentModeKHR SelectSwapPresentMode( const SwapChainSupportDetails& details, VkPresentModeKHR preferred )
{
    const auto default_present_mode = VK_PRESENT_MODE_MAILBOX_KHR; 

    for( auto available : details.present_modes ) { if( available == preferred ) return available; }
    AE_WARN( "Preferred present mode unavailable, trying default" );
    for( auto available : details.present_modes ) { if( available == default_present_mode ) return available; }
    AE_WARN( "Preferred present mode unavailable, using fallback" );
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SelectSwapExtent( const SwapChainSupportDetails& details, uint32_t width, uint32_t height )
{
    const VkSurfaceCapabilitiesKHR& capabilities = details.capabilities;

    if( capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() ) return capabilities.currentExtent;
    else return VkExtent2D
    {
        std::max( capabilities.minImageExtent.width, std::min( capabilities.maxImageExtent.width, width ) ),
        std::max( capabilities.minImageExtent.height, std::min( capabilities.maxImageExtent.height, height ) )
    };
}

}// namespace AEON::Graphics::vk
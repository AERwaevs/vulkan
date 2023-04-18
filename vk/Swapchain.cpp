#include "Swapchain.h"

namespace AEON::Graphics::vk
{

Swapchain::Swapchain( PhysicalDevice* physicalDevice, Device* device, Surface* surface, uint32_t width, uint32_t height, SwapchainPreferences& preferences, ref_ptr<Swapchain> old )
: _device{ device }, _surface{ surface }
{
    const auto& details{ QuerySwapchainSupport( *physicalDevice, *surface ) };
    const auto& extent{ SelectSwapExtent( details, width, height ) };
    const auto& surfaceFormat{ SelectSwapSurfaceFormat( details, preferences.surfaceFormat ) };
    const auto& presentMode{ SelectSwapPresentMode( details, preferences.presentMode ) };
    const auto& minImageCount{ std::max( preferences.imageCount, details.capabilities.minImageCount + 1 ) };
    const auto& imageCount
    {
        details.capabilities.maxImageCount > 0
            ? std::min( minImageCount, details.capabilities.maxImageCount )
            : std::max( preferences.imageCount, minImageCount )
    };
    auto [ graphicsFamily, presentFamily ]{ physicalDevice->GetQueueFamilies( VK_QUEUE_GRAPHICS_BIT, surface ) };
    uint32_t queueFamilyIndices[]{ graphicsFamily, presentFamily };

    preferences.imageCount    = imageCount;
    preferences.presentMode   = presentMode;
    preferences.surfaceFormat = surfaceFormat;

    VkSwapchainCreateInfoKHR createInfo
    {
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        VK_NULL_HANDLE, //pNext
        VkSwapchainCreateFlagsKHR{ 0 },
        *surface,
        imageCount,
        surfaceFormat.format,
        surfaceFormat.colorSpace,
        extent,
        1, //imageArrayLayers //* this is only > 1 if swapchain is stereoscopic
        preferences.imageUsage,
        graphicsFamily != presentFamily ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
        graphicsFamily != presentFamily ? 2u : 0u, //pQueueFamilyIndices
        graphicsFamily != presentFamily ? queueFamilyIndices : VK_NULL_HANDLE,
        details.capabilities.currentTransform,
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        presentMode,
        VK_TRUE,    //clipped
        old ? *old : VK_NULL_HANDLE
    };

    auto result = vkCreateSwapchainKHR( *device, &createInfo, VK_ALLOCATOR, &_swapchain );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create swapchain: vk%d", result );
    AE_INFO( "Swapchain created" );
}

Swapchain::~Swapchain()
{
    vkDestroySwapchainKHR( *_device, _swapchain, VK_ALLOCATOR );
}

SwapchainSupportDetails QuerySwapchainSupport( VkPhysicalDevice device, VkSurfaceKHR surface )
{
    SwapchainSupportDetails details;

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

VkSurfaceFormatKHR SelectSwapSurfaceFormat( const SwapchainSupportDetails& details, VkSurfaceFormatKHR preferred )
{
    const auto  default_format      = SwapchainPreferences::default_format;
    const auto  default_colorspace  = SwapchainPreferences::default_colorspace;

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

VkPresentModeKHR SelectSwapPresentMode( const SwapchainSupportDetails& details, VkPresentModeKHR preferred = VK_PRESENT_MODE_MAILBOX_KHR )
{
    const auto default_present_mode = SwapchainPreferences::default_present_mode; 

    for( auto available : details.present_modes ) { if( available == preferred ) return available; }
    AE_WARN( "Preferred present mode unavailable, trying default" );
    for( auto available : details.present_modes ) { if( available == default_present_mode ) return available; }
    AE_WARN( "Preferred present mode unavailable, using fallback" );
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SelectSwapExtent( const SwapchainSupportDetails& details, uint32_t width, uint32_t height )
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
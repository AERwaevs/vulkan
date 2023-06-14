#include "Swapchain.h"

namespace aer::gfx::vk
{

Swapchain::Swapchain
(
    ref_ptr<PhysicalDevice> physical_device, ref_ptr<Device> device, ref_ptr<Surface> surface,
    uint32_t width, uint32_t height, SwapchainPreferences& preferences, ref_ptr<Swapchain> old
)
: _device( device ), _surface( surface )
{
    const auto details          = QuerySwapchainSupport( *physical_device, *surface );
    const auto extent           = SelectSwapExtent( details, width, height );
    const auto surfaceFormat    = SelectSwapSurfaceFormat( details, preferences.surfaceFormat );
    const auto presentMode      = SelectSwapPresentMode( details, preferences.presentMode );
    const auto imageArrayLayers = 1u; //* imageArrayLayers is only > 1 if swapchain is stereoscopic
          auto imageCount       = std::max( preferences.imageCount, details.capabilities.minImageCount + 1 );
               imageCount       = details.capabilities.maxImageCount > 0
                                ? std::min( imageCount, details.capabilities.maxImageCount )
                                : imageCount;
    auto [ graphicsFamily, presentFamily ]{ physical_device->GetQueueFamilies( VK_QUEUE_GRAPHICS_BIT, surface.get() ) };
    const std::vector<uint32_t> queueFamilyIndices
    {
        static_cast<uint32_t>( graphicsFamily ),
        static_cast<uint32_t>( presentFamily )
    };
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
        imageArrayLayers, 
        preferences.imageUsage,
        graphicsFamily == presentFamily ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
        graphicsFamily == presentFamily ? 1u : static_cast<uint32_t>( queueFamilyIndices.size() ),
        queueFamilyIndices.data(),
        details.capabilities.currentTransform,
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        presentMode,
        VK_TRUE,    //clipped
        old ? *old : VK_NULL_HANDLE
    };

    auto result = vkCreateSwapchainKHR( *device, &createInfo, VK_ALLOCATOR, &_swapchain );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create swapchain: %s", ResultMessage( result ) );

    _extent = extent;
    _format = surfaceFormat.format;

    vkGetSwapchainImagesKHR( *device, _swapchain, &imageCount, VK_NULL_HANDLE );
    std::vector<VkImage> images( imageCount );
    vkGetSwapchainImagesKHR( *device, _swapchain, &imageCount, images.data() );

    for( std::size_t i = 0; i < images.size(); i++ )
    {
        auto image_view = ImageView::create( Image::create( images[i], device.get() ) );

        image_view->image->usage                    = preferences.imageUsage;
        image_view->image->extent                   = { _extent.width, _extent.height, 1 };
        image_view->image->arrayLayers              = imageArrayLayers;
        image_view->image->format                   = _format;
        image_view->image->mipLevels                = 1;

        image_view->viewType                        = VK_IMAGE_VIEW_TYPE_2D;
        image_view->format                          = surfaceFormat.format;
        image_view->subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view->subresourceRange.baseMipLevel   = 0;
        image_view->subresourceRange.levelCount     = 1;
        image_view->subresourceRange.baseArrayLayer = 0;
        image_view->subresourceRange.layerCount     = 1;
        image_view->Compile( device.get() );

        _views.push_back( image_view );
    }
}

Swapchain::~Swapchain()
{
    AE_INFO( "Destroying VkSwapchain" );
    vkDestroySwapchainKHR( *_device, _swapchain, VK_ALLOCATOR );
}

SwapchainSupportDetails QuerySwapchainSupport( VkPhysicalDevice device, VkSurfaceKHR surface )
{
    SwapchainSupportDetails details{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR( device, surface, &details.capabilities );

    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR( device, surface, &format_count, VK_NULL_HANDLE );

    if( format_count != 0 )
    {
        details.formats.resize( format_count );
        vkGetPhysicalDeviceSurfaceFormatsKHR( device, surface, &format_count, details.formats.data() );
    }

    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR( device, surface, &present_mode_count, VK_NULL_HANDLE );

    if( present_mode_count != 0 )
    {
        details.present_modes.resize( present_mode_count );
        vkGetPhysicalDeviceSurfacePresentModesKHR( device, surface, &present_mode_count, details.present_modes.data() );
    }

    return details;
}

VkSurfaceFormatKHR SelectSwapSurfaceFormat( const SwapchainSupportDetails& details, VkSurfaceFormatKHR preferred )
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

VkPresentModeKHR SelectSwapPresentMode( const SwapchainSupportDetails& details, VkPresentModeKHR preferred = VK_PRESENT_MODE_MAILBOX_KHR )
{
    const auto default_present_mode = VK_PRESENT_MODE_FIFO_KHR;

    for( auto available : details.present_modes ) { if( available == preferred ) return available; }
    AE_WARN( "Preferred present mode unavailable, trying default" );
    for( auto available : details.present_modes ) { if( available == default_present_mode ) return available; }
    AE_WARN( "Default present mode unavailable, using fallback" );
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SelectSwapExtent( const SwapchainSupportDetails& details, uint32_t width, uint32_t height )
{
    const auto& capabilities = details.capabilities;

    if( capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)() ) return capabilities.currentExtent;
    else return VkExtent2D
    {
        std::max( capabilities.minImageExtent.width, std::min( capabilities.maxImageExtent.width, width ) ),
        std::max( capabilities.minImageExtent.height, std::min( capabilities.maxImageExtent.height, height ) )
    };
}

}// namespace aer::gfx::vk
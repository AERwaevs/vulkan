#pragma once

#include <Base/Base.h>
#include <glad/vulkan.h>

#include "LogicalDevice.h"
#include "Surface.h"

namespace AEON::Graphics::vk
{

struct SwapchainSupportDetails
{
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;
};

struct SwapchainPreferences
{
    static constexpr uint32_t          default_image_count  = 3;                                    // triple buffering
    static constexpr VkFormat          default_format       = VK_FORMAT_B8G8R8A8_UNORM;             // 8bits per channel, unsigned float 
    static constexpr VkColorSpaceKHR   default_colorspace   = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;    // SRGB
    static constexpr VkPresentModeKHR  default_present_mode = VK_PRESENT_MODE_FIFO_KHR;             // vysnc on
    static constexpr VkImageUsageFlags default_image_usage  = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;  // render direct to image

    uint32_t            imageCount{    default_image_count };
    VkSurfaceFormatKHR  surfaceFormat{ default_format, default_colorspace };
    VkPresentModeKHR    presentMode{   default_present_mode };
    VkImageUsageFlags   imageUsage{    default_image_usage };
};

SwapchainSupportDetails QuerySwapchainSupport( VkPhysicalDevice device, VkSurfaceKHR surface );
VkSurfaceFormatKHR      SelectSwapSurfaceFormat( const SwapchainSupportDetails& details, VkSurfaceFormatKHR preferred );
VkPresentModeKHR        SelectSwapPresentMode(   const SwapchainSupportDetails& details, VkPresentModeKHR preferred );
VkExtent2D              SelectSwapExtent(        const SwapchainSupportDetails& details, uint32_t width, uint32_t height );

class Swapchain : public ICreatable< Swapchain >
{
private:
    ref_ptr<Device>     _device;
    ref_ptr<Surface>    _surface;

    VkSwapchainKHR      _swapchain;
    VkFormat            _format;
    VkExtent2D          _extent;

public:
    Swapchain( PhysicalDevice* physical_device, Device* device, Surface* surface, uint32_t width, uint32_t height, SwapchainPreferences& preferences, ref_ptr<Swapchain> old = {} );
    operator VkSwapchainKHR() const { return _swapchain; }
    ~Swapchain();
};

}// namespace AEON::Graphics::vk
#pragma once

#include <glad/vulkan.h>

#include "LogicalDevice.h"
#include "Surface.h"
#include "state/Image.h"
#include "state/ImageView.h"

namespace aer::Graphics::vk
{

struct SwapchainSupportDetails
{
    VkSurfaceCapabilitiesKHR        capabilities{};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;
};

struct SwapchainPreferences
{
    // defaults to triple buffering
    uint32_t            imageCount = 3u;
    // defaults to 8bits per channel unsigned float, SRGB
    VkSurfaceFormatKHR  surfaceFormat = VkSurfaceFormatKHR{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
    // defaults to vysnc on
    VkPresentModeKHR    presentMode = VK_PRESENT_MODE_FIFO_KHR;
    // defaults to render direct to image
    VkImageUsageFlags   imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
};

SwapchainSupportDetails QuerySwapchainSupport( VkPhysicalDevice device, VkSurfaceKHR surface );
VkSurfaceFormatKHR      SelectSwapSurfaceFormat( const SwapchainSupportDetails& details, VkSurfaceFormatKHR preferred );
VkPresentModeKHR        SelectSwapPresentMode(   const SwapchainSupportDetails& details, VkPresentModeKHR preferred );
VkExtent2D              SelectSwapExtent(        const SwapchainSupportDetails& details, uint32_t width, uint32_t height );

class Swapchain : public virtual Object, public Interfaces< Swapchain, ICreate >
{
public:
    Swapchain
    (
        ref_ptr<PhysicalDevice> physical_device, ref_ptr<Device> device, ref_ptr<Surface> surface,
        uint32_t width, uint32_t height, SwapchainPreferences& preferences, ref_ptr<Swapchain> old = {}
    );
    operator VkSwapchainKHR() const { return _swapchain; }
    ~Swapchain();
private:
    ref_ptr<Device>         _device;
    ref_ptr<Surface>        _surface;
    Images                  _images;
    ImageViews              _views;

    VkSwapchainKHR          _swapchain;
    VkFormat                _format;
    VkExtent2D              _extent;
};

}// namespace aer::Graphics::vk
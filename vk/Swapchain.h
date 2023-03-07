#pragma once

#include <Base/Base.h>
#include <glad/vulkan.h>

#include <Core/Log.h>

namespace AEON::Graphics::vk
{

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;
};

SwapChainSupportDetails QuerySwapchainSupport( VkPhysicalDevice device, VkSurfaceKHR surface );
VkSurfaceFormatKHR      SelectSwapSurfaceFormat( const  SwapChainSupportDetails& details, 
                                                        VkSurfaceFormatKHR preferred );
VkPresentModeKHR        SelectSwapPresentMode(   const  SwapChainSupportDetails& details, 
                                                        VkPresentModeKHR preferred );
VkExtent2D              SelectSwapExtent(        const  SwapChainSupportDetails& details,
                                                        uint32_t width, uint32_t height );

}// namespace AEON::Graphics::vk
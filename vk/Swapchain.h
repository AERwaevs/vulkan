#pragma once

#include <glad/vulkan.h>

namespace AEON::Graphics::vk
{

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;
};

SwapChainSupportDetails QuerySwapchainSupport( VkPhysicalDevice device, VkSurfaceKHR surface );

}// namespace AEON::Graphics::vk
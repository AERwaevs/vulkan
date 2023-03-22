#pragma once

#include <Base/Base.h>

#include <Graphics/Viewport.h>

#include "VulkanRenderer.h"
#include "vk/Instance.h"
#include "vk/Surface.h"
#include "vk/LogicalDevice.h"
#include "vk/Swapchain.h"

namespace AEON::Graphics
{
    class VulkanViewport : public Viewport
    {
    public:
        VulkanViewport( Window* window );
        ~VulkanViewport();
    private:
        Shared<vk::Instance>        m_instance;
        Shared<vk::Surface>         m_surface;
        Shared<vk::PhysicalDevice>  m_physical_device;
        Shared<vk::Device>          m_device;
        Shared<vk::Swapchain>       m_swapchain;
        
        vk::SwapchainPreferences    m_swapchain_prefs;
        VkExtent2D                  m_extent;

    };
} // namespace AEON::Graphics

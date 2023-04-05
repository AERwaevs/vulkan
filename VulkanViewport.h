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
    class VulkanViewport : public Viewport, public ICreatable< VulkanViewport >
    {
    public:
        VulkanViewport( Window* window );
        ~VulkanViewport();
    private:
        
        
        ref_ptr<vk::Instance>        _instance;
        ref_ptr<vk::Surface>         _surface;
        ref_ptr<vk::PhysicalDevice>  _physical_device;
        ref_ptr<vk::Device>          _device;
        ref_ptr<vk::Swapchain>       _swapchain;
        
        vk::SwapchainPreferences    _swapchain_prefs;
        VkExtent2D                  _extent;

    };
} // namespace AEON::Graphics

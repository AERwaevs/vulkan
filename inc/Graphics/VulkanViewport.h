#pragma once

#include <Graphics/Viewport.h>
#include <Graphics/Window.h>

#include "VulkanRenderer.h"
#include "vk/Instance.h"
#include "vk/Surface.h"
#include "vk/LogicalDevice.h"
#include "vk/Swapchain.h"

namespace AEON::Graphics
{
    class Window;
    
    class VulkanViewport : public Viewport, public Interfaces< VulkanViewport, ICreateIf >
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
    };
} // namespace AEON::Graphics

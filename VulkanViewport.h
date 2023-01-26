#pragma once

#include <Graphics/Viewport.h>

#include <vk/Surface.h>
#include <vk/LogicalDevice.h>

namespace AEON::Graphics
{
    class VulkanViewport : public Inherit< Viewport, VulkanViewport >
    {
    public:
        VulkanViewport( Renderer* renderer, vk::Surface* surface );
        ~VulkanViewport();
    private:
        Shared<vk::Surface> m_surface;
        Shared<vk::Device>  m_device;
    };
    using Viewports = List<Shared<Viewport>>;
    
} // namespace AEON::Graphics

#pragma once

#include <Graphics/Viewport.h>

#include <vk/Instance.h>
#include <vk/Surface.h>
#include <vk/LogicalDevice.h>

namespace AEON::Graphics
{
    class VulkanViewport : public Viewport
    {
    public:
        static Shared<VulkanViewport> create();
        VulkanViewport();
        ~VulkanViewport();
    private:
        Shared<vk::Instance>    m_instance;
        Shared<vk::Surface>     m_surface;
        Shared<vk::Device>      m_device;
    };
} // namespace AEON::Graphics

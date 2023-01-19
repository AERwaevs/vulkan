#pragma once

#include <Graphics/Renderer.h>

#include <vk/Instance.h>
#include <vk/Surface.h>
#include <vk/LogicalDevice.h>

namespace AEON::Graphics
{
    class AEON_DLL VulkanRenderer : public Inherit< Renderer, VulkanRenderer >
    {
    public:
        VulkanRenderer();
    protected:
        virtual ~VulkanRenderer();
    private:
        Shared<vk::Instance>        m_instance;
        List<Shared<vk::Device>>    m_devices;
        List<Shared<vk::Surface>>   m_surfaces;
    };
}
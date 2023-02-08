#pragma once

#include <Graphics/Renderer.h>

#include <vk/Instance.h>
#include <vk/Surface.h>
#include <vk/LogicalDevice.h>

namespace AEON::Graphics
{
    class AEON_DLL VulkanRenderer : public Renderer
    {
    public:
                                VulkanRenderer();
        Shared<vk::Instance>    instance() const { return m_instance; }
    protected:
        virtual         ~VulkanRenderer();
    private:
        Shared<vk::Instance>        m_instance;
    };
}
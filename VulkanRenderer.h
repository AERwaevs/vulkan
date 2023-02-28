#pragma once

#include <Graphics/Renderer.h>

#include <vk/Extensions.h>
#include <vk/Layers.h>
#include <vk/Instance.h>
#include <vk/Surface.h>
#include <vk/LogicalDevice.h>

namespace AEON::Graphics
{
    class AEON_DLL VulkanRenderer : public Renderer
    {
    public:
        VulkanRenderer( Vector<const char*> extensions = vk::GetRequiredExtensions(),
                        Vector<const char*> layers = vk::GetRequiredLayers() );
        Shared<vk::Instance> vkinstance() const { return m_instance; }
    protected:
        virtual         ~VulkanRenderer();
    private:
        Shared<vk::Instance>        m_instance;
    };
}
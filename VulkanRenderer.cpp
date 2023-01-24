#include <VulkanRenderer.h>

#include <vk/Extensions.h>
#include <vk/Layers.h>

namespace AEON::Graphics
{
    AEON_API VulkanRenderer::VulkanRenderer()
    :   m_instance( vk::Instance::create
        (
            vk::GetRequiredExtensions(),
            vk::GetRequiredLayers ()
        ) )
    {
        //m_surfaces.push_back
        //(
        //    new vk::Surface( m_instance, )
        //);
        //m_devices.push_back
        //(
        //    m_instance->GetPhysicalDevices().front()

        //);
    }

    AEON_API VulkanRenderer::~VulkanRenderer()
    {
        gladLoaderUnloadVulkan();
    }
}
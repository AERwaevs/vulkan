#include <VulkanRenderer.h>

#include <vk/Extensions.h>
#include <vk/Layers.h>

namespace AEON::Graphics
{
    AEON_API VulkanRenderer::VulkanRenderer()
    {
        m_instance ? m_instance : vk::Instance::create( vk::GetRequiredExtensions(),
                                                        vk::GetRequiredLayers () );
    }

    AEON_API VulkanRenderer::~VulkanRenderer()
    {

    }
}
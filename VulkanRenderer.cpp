#include <VulkanRenderer.h>

#include <vk/Extensions.h>
#include <vk/Layers.h>
#include <vk/Instance.h>

namespace AEON::Graphics
{
    
AEON_API VulkanRenderer::VulkanRenderer()
:   m_instance( new vk::Instance
    (
        vk::GetRequiredExtensions(),
        vk::GetRequiredLayers ()
    ) )
{

}

AEON_API VulkanRenderer::~VulkanRenderer()
{
    gladLoaderUnloadVulkan();
}

}
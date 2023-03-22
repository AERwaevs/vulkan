#include <VulkanRenderer.h>

namespace AEON::Graphics
{
    
AEON_API VulkanRenderer::VulkanRenderer()
: Renderer{ Vulkan }, m_instance( vk::Instance::instance() )
{

}

AEON_API VulkanRenderer::~VulkanRenderer()
{
    
}

}
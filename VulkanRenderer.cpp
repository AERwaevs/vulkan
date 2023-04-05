#include <VulkanRenderer.h>

namespace AEON::Graphics
{
    
AEON_API VulkanRenderer::VulkanRenderer()
: Renderer{ Vulkan }, _vkinstance( vk::Instance::instance() )
{

}

AEON_API VulkanRenderer::~VulkanRenderer()
{
    
}

}
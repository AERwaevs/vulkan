#include <VulkanRenderer.h>

namespace AEON::Graphics
{
    
AEON_API VulkanRenderer::VulkanRenderer()
: Renderer{ Vulkan }, _vkinstance( vk::Instance::instance() )
{
    AE_INFO( "VulkanRenderer created" );
}

AEON_API VulkanRenderer::~VulkanRenderer()
{
    AE_INFO( "VulkanRenderer destroyed" );
}

}
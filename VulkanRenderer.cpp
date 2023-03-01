#include <VulkanRenderer.h>

namespace AEON::Graphics
{
    
AEON_API VulkanRenderer::VulkanRenderer( Vector<const char*> extensions, Vector<const char*> layers )
: Renderer{ Vulkan }, m_instance( vk::Instance::create( extensions, layers ) )
{

}

AEON_API VulkanRenderer::~VulkanRenderer()
{
    
}

}
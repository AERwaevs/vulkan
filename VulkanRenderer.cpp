#include <VulkanRenderer.h>

namespace AEON::Graphics
{

template<> ref_ptr<Renderer> Renderer::create< API::Vulkan >()
{
    return VulkanRenderer::create_if( VulkanSupported() );
};

AEON_API VulkanRenderer::VulkanRenderer()
: Renderer{ Vulkan }, _vkinstance( vk::Instance::create() )
{
    AE_INFO( "VulkanRenderer created" );
}

AEON_API VulkanRenderer::~VulkanRenderer()
{
    AE_INFO( "VulkanRenderer destroyed" );
}

}
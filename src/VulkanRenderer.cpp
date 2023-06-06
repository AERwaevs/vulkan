#include <Graphics/VulkanRenderer.h>

namespace aer::Graphics
{

template<> ref_ptr<Renderer> Renderer::create< API::Vulkan >()
{
    return VulkanRenderer::create_if( VulkanSupported() );
}

AEON_API VulkanRenderer::VulkanRenderer()
: _vkinstance( vk::Instance::get_or_create() )
{
    AE_INFO( "VulkanRenderer created" );
}

AEON_API VulkanRenderer::~VulkanRenderer()
{
    AE_INFO( "VulkanRenderer destroyed" );
}

}
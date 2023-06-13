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
}

AEON_API VulkanRenderer::~VulkanRenderer()
{
}

}
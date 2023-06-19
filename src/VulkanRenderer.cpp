#include <Graphics/VulkanRenderer.h>

namespace aer::gfx
{

template<> ref_ptr<Renderer> Renderer::create< API::Vulkan >()
{
    return VulkanRenderer::create_if( VulkanSupported() );
}

VulkanRenderer::VulkanRenderer()
: _vkinstance( vk::Instance::get_or_create() )
{
}

}
#include <Graphics/VulkanRenderer.h>

namespace aer::gfx
{

template<> ref_ptr<Renderer> Renderer::create< API::Vulkan >()
{
    return VulkanRenderer::create_if( ::vk_supported() );
}

VulkanRenderer::VulkanRenderer()
{
}

}
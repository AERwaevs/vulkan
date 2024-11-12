#include <Graphics/VulkanRenderer.h>

namespace aer::gfx
{

template<> ref_ptr<Renderer> Renderer::get_or_create< API::Vulkan >()
{
    return aer::create<VulkanRenderer>();
}

VulkanRenderer::VulkanRenderer()
{
}

}
#include <Graphics/VulkanRenderer.h>

namespace aer::gfx
{

template<> ref_ptr<Renderer> Renderer::create< API::Vulkan >()
{
    return aer::create<VulkanRenderer>();
}

VulkanRenderer::VulkanRenderer()
{
}

}
#include <VulkanViewport.h>

namespace AEON::Graphics
{   

VulkanViewport::VulkanViewport( Renderer* renderer, vk::Surface* surface )
: Inherit{ renderer }, m_surface{ surface }
{
}

VulkanViewport::~VulkanViewport()
{
}

} // namespace AEON::Graphics

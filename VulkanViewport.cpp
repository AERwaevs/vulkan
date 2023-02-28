#include <VulkanViewport.h>
#include <VulkanRenderer.h>

#include <Core/Window.h>

namespace AEON::Graphics
{

VulkanViewport::VulkanViewport( Window* window )
: Viewport{ window }, m_instance{ vk::Instance::get_or_create() },
  m_surface{ new vk::Surface( m_instance, window ) },
  m_device{ new vk::Device( m_instance->physical_devices().front(), m_surface ) }
{
    
}

VulkanViewport::~VulkanViewport()
{
}

} // namespace AEON::Graphics

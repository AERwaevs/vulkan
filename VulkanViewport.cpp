#include <VulkanViewport.h>
#include <VulkanRenderer.h>

namespace AEON::Graphics
{

VulkanViewport::VulkanViewport( Window* window )
: Viewport{ window },
  m_instance{ vk::Instance::instance() },
  m_surface{ vk::Surface::create( m_instance, window ) },
  m_physical_device{ m_instance->physical_devices().front() },  //TODO remove hardcoded physical device selection
  m_device{ vk::Device::create( m_physical_device, m_surface ) },
  m_swapchain_prefs{}
  //m_swapchain{ vk::Swapchain::create( m_physical_device.get(), m_device.get(), m_surface.get(), window->width(), window->height(), m_swapchain_prefs ) }
{
    
}

VulkanViewport::~VulkanViewport()
{
}

} // namespace AEON::Graphics

#include <VulkanViewport.h>
#include <VulkanRenderer.h>

namespace AEON::Graphics
{

template<> ref_ptr<Viewport> Viewport::create< API::Vulkan >( Window* window )
{
    return VulkanViewport::create_if( VulkanSupported(), window );
}

VulkanViewport::VulkanViewport( Window* window )
: Viewport{ window },
  _instance{ vk::Instance::create() },
  _surface{ vk::Surface::create( _instance, window ) },
  _physical_device{ _instance->physical_devices().front() },  //TODO remove hardcoded physical device selection
  _device{ vk::Device::create( _physical_device, _surface ) },
  _swapchain_prefs{},
  _swapchain{ vk::Swapchain::create( _physical_device.get(), _device.get(), _surface.get(), window->width(), window->height(), _swapchain_prefs ) }
{
    AE_INFO( "VulkanViewport created" );
}

VulkanViewport::~VulkanViewport()
{
    AE_INFO( "VulkanViewport destroyed" );
}

} // namespace AEON::Graphics

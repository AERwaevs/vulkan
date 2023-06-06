#include <Graphics/VulkanViewport.h>
#include <Graphics/VulkanRenderer.h>

namespace aer::Graphics
{

template<>
ref_ptr<Viewport> Viewport::create< API::Vulkan >( Window* window )
{
    return VulkanViewport::create_if( VulkanSupported(), window );
}

const auto GetQueueSettings( ref_ptr<vk::PhysicalDevice> physical_device, ref_ptr<vk::Surface> surface )
{
    auto [ present_family, graphics_family ]
    {
        physical_device->GetQueueFamilies( VK_QUEUE_GRAPHICS_BIT, surface.get() )
    };
    return vk::QueueSettings
    {
        vk::QueueSetting{ graphics_family,{ 1.0f } },
        vk::QueueSetting{ present_family, { 1.0f } },
    };
}

VulkanViewport::VulkanViewport( Window* window )
:   Viewport( window, VulkanRenderer::get_or_create().get() ),
    _instance( vk::Instance::get_or_create() ),
    _surface( vk::Surface::create( _instance, window ) ),
    _physical_device( _instance->physical_devices().front() ),  //TODO remove hardcoded physical device selection
    _device( vk::Device::create
    (
      _physical_device, _surface, GetQueueSettings( _physical_device, _surface )
    ) ),
    _swapchain_prefs( vk::SwapchainPreferences::defaults() ),
    _swapchain( vk::Swapchain::create
    (
      _physical_device, _device, _surface,
      window->width(), window->height(), _swapchain_prefs
    ) )
{
    AE_INFO( "VulkanViewport created" );
}

VulkanViewport::~VulkanViewport()
{
    AE_INFO( "VulkanViewport destroyed" );
}

} // namespace aer::Graphics

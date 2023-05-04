#include <VulkanViewport.h>
#include <VulkanRenderer.h>

namespace AEON::Graphics
{

template<> ref_ptr<Viewport> Viewport::create< API::Vulkan >( Window* window )
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
:   Viewport( window ),
    _instance( vk::Instance::instance() ),
    _surface( vk::Surface::create( _instance, window ) ),
    _physical_device( _instance->physical_devices().front() ),  //TODO remove hardcoded physical device selection
    _device( vk::Device::create
    (
      _physical_device, _surface, GetQueueSettings( _physical_device, _surface )
    ) ),
    _swapchain_prefs()
    //_swapchain( vk::Swapchain::create
    //(
    //  _physical_device, _device, _surface,
    //  window->width(), window->height(), _swapchain_prefs
    //) )
{
    //vk::SwapchainPreferences prefs = vk::SwapchainPreferences();
    //auto swapchain = vk::Swapchain::create( _physical_device, _device, _surface, 1280, 720, prefs );
    AE_INFO( "VulkanViewport created without swapchain" );
}

VulkanViewport::~VulkanViewport()
{
    AE_INFO( "VulkanViewport destroyed" );
}

} // namespace AEON::Graphics

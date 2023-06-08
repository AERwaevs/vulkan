#include <Graphics/VulkanViewport.h>
#include <Graphics/VulkanRenderer.h>

namespace aer::Graphics
{

template<>
ref_ptr<Viewport> Viewport::create< API::Vulkan >( Window* window )
{
    return VulkanViewport::create_if( VulkanSupported(), window );
}

const inline auto GetQueueSettings( vk::PhysicalDevice* physical_device, vk::Surface* surface )
{
    auto [ present_family, graphics_family ]
    {
        physical_device->GetQueueFamilies( VK_QUEUE_GRAPHICS_BIT, surface )
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
    _physical_device( _instance->physical_device( VK_QUEUE_GRAPHICS_BIT, _surface.get(), _instance->DevicePreferences ) ),
    _device( vk::Device::create
    (
        _physical_device, _surface, GetQueueSettings( _physical_device.get(), _surface.get() )
    ) ),
    _swapchain_prefs()
{
    //? if _swapchain is created during initialisation, it seems to execute before _swapchain_prefs is constructed
    _swapchain = vk::Swapchain::create
    (
        _physical_device, _device, _surface,
        window->width(), window->height(), _swapchain_prefs
    );
    AE_INFO( "VulkanViewport created" );
}

VulkanViewport::~VulkanViewport()
{
    AE_INFO( "VulkanViewport destroyed" );
}

} // namespace aer::Graphics

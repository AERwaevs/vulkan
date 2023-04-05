#include <glad/vulkan.h>

#include "Instance.h"
#include "PhysicalDevice.h"
#include "Swapchain.h"

namespace AEON::Graphics::vk
{

PhysicalDevice::PhysicalDevice( Instance* instance, VkPhysicalDevice device )
: _instance( instance ), _device( device )
{
    vkGetPhysicalDeviceProperties( device, &_properties );
    vkGetPhysicalDeviceFeatures( device, &_features );
    
    uint32_t queue_family_count{ 0 };
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queue_family_count, VK_NULL_HANDLE );

    _queue_families.resize( queue_family_count );
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queue_family_count, _queue_families.data() );

    instance->GetProcAddr( GetPhysicalDeviceFeatures2, "vkGetPhysicalDeviceFeatures2" );
    instance->GetProcAddr( GetPhysicalDeviceProperties2, "vkGetPhysicalDeviceFeatures2" );
}

auto PhysicalDevice::EnumerateExtensionProperties( Name layer_name ) const
{
    VkResult result{ VK_SUCCESS };
    uint32_t extension_count{ 0 };
    result = vkEnumerateDeviceExtensionProperties( _device,
                                                   layer_name, 
                                                   &extension_count, 
                                                   nullptr );
    AE_ERROR_IF( result != VK_SUCCESS, 
                    "Failed to enumerate device extension count: vk%d", result );

    Vector<VkExtensionProperties> vk_extensions(extension_count);
    result = vkEnumerateDeviceExtensionProperties( _device,
                                                   layer_name, 
                                                   &extension_count, 
                                                   vk_extensions.data() );
    AE_ERROR_IF( result != VK_SUCCESS, 
                    "Failed to enumerate device extension properties: vk%d", result );

    return vk_extensions;
}

bool PhysicalDevice::Supported() const
{
    auto extensions_supported = [&]()
    {
        std::set<std::string> required
        {
            PhysicalDevice::RequiredExtensions().begin(),
            PhysicalDevice::RequiredExtensions().end()
        };

        for( const auto& extension : PhysicalDevice::EnumerateExtensionProperties() )
        {
            required.erase( extension.extensionName );
        }

        return required.empty();
    };

    bool   supported( false );
           supported |= _properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
           supported |= _properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
           supported &= GetQueueFamily( VK_QUEUE_GRAPHICS_BIT );
           supported &= extensions_supported();
    return supported;
}

uint32_t PhysicalDevice::GetQueueFamily( VkQueueFlags flags ) const
{
    uint32_t family{ 0xFFFFFFFF };
    for( uint32_t i = 0; i < _queue_families.size(); i++ )
    {
        if( _queue_families[i].queueFlags == flags ) return i;     // perfect match
        if( _queue_families[i].queueFlags && flags ) family = i;   // best match
    }
    return family; 
}

std::pair<uint32_t, uint32_t> PhysicalDevice::GetQueueFamilies( VkQueueFlags flags, Surface* surface ) const
{
    uint32_t queueFamily{   0xFFFFFFFF };
    uint32_t presentFamily{ 0xFFFFFFFF };

    for( uint32_t i = 0; i < _queue_families.size(); i++ )
    {
        VkBool32 present_supported{ false };
        vkGetPhysicalDeviceSurfaceSupportKHR( _device, i, *surface, &present_supported );

        bool matched{ ( _queue_families[i].queueFlags & flags ) == flags };

        if( matched && present_supported ) return { i, i };
        if( matched )           queueFamily = i;
        if( present_supported ) presentFamily = i;
    }

    return { queueFamily, presentFamily }; 
}

}
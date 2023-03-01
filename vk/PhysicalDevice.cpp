#include <glad/vulkan.h>

#include <vk/Instance.h>
#include <vk/PhysicalDevice.h>

namespace AEON::Graphics::vk
{

PhysicalDevice::PhysicalDevice( Instance* instance, VkPhysicalDevice device )
: m_instance( instance ), m_device( device )
{
    vkGetPhysicalDeviceProperties( device, &m_properties );
    vkGetPhysicalDeviceFeatures( device, &m_features );
    
    uint32_t queue_family_count{ 0 };
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queue_family_count, VK_NULL_HANDLE );

    m_queue_families.resize( queue_family_count );
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queue_family_count, m_queue_families.data() );

    instance->GetProcAddr( GetPhysicalDeviceFeatures2, "vkGetPhysicalDeviceFeatures2" );
    instance->GetProcAddr( GetPhysicalDeviceProperties2, "vkGetPhysicalDeviceFeatures2" );
}

auto PhysicalDevice::EnumerateExtensionProperties( Name layer_name ) const
{
    VkResult result{ VK_SUCCESS };
    uint32_t extension_count{ 0 };
    result = vkEnumerateDeviceExtensionProperties( m_device,
                                                   layer_name, 
                                                   &extension_count, 
                                                   nullptr );
    AE_ERROR_IF( result != VK_SUCCESS, 
                    "Failed to enumerate device extension count: vk%d", result );

    Vector<VkExtensionProperties> vk_extensions(extension_count);
    result = vkEnumerateDeviceExtensionProperties( m_device,
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
            RequiredExtensions().begin(),
            RequiredExtensions().end()
        };

        for( const auto& extension : EnumerateExtensionProperties() )
        {
            required.erase( extension.extensionName );
        }

        return required.empty();
    };

    bool   supported( false );
           supported |= m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
           supported |= m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
           supported &= GetQueueFamily( VK_QUEUE_GRAPHICS_BIT );
           supported &= extensions_supported();
    return supported;
}

uint32_t PhysicalDevice::Capability() const
{
    if( !Supported() ) return 0;
    int     score{ 0 };
            score += m_properties.limits.maxImageDimension2D;
            score += m_features.geometryShader;
            score += m_features.shaderFloat64;
            score += m_features.largePoints;
            score += m_features.multiDrawIndirect;
            score += m_features.variableMultisampleRate;
    return  score; 
}

uint32_t PhysicalDevice::GetQueueFamily( VkQueueFlags flags ) const
{
    uint32_t family{ 0xFFFFFFFF };
    for( uint32_t i = 0; i < m_queue_families.size(); i++ )
    {
        if( m_queue_families[i].queueFlags == flags ) return i;     // perfect match
        if( m_queue_families[i].queueFlags && flags ) family = i;   // best match
    }
    return family; 
}

uint32_t PhysicalDevice::GetQueueFamily( VkQueueFlags flags, Surface* surface ) const
{
    uint32_t family{ 0xFFFFFFFF };
    for( uint32_t i = 0; i < m_queue_families.size(); i++ )
    {
        VkBool32 present_supported{ false };
        vkGetPhysicalDeviceSurfaceSupportKHR( m_device, i, *surface, &present_supported );

        if( present_supported )
        {
            if( m_queue_families[i].queueFlags == flags ) return i;
            if( m_queue_families[i].queueFlags && flags ) family = i;
        }
    }
    return family; 
}

}
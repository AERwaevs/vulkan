#include <vk/Instance.h>
#include <vk/Surface.h>
#include <vk/PhysicalDevice.h>

namespace aer::gfx::vk
{

#ifndef NDEBUG
VKAPI_ATTR VkBool32 debug_callback
(
    VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
    VkDebugUtilsMessageTypeFlagsEXT             type,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void*                                       pUserData
)
{
    switch ( severity )
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        {
            AE_INFO( "%s", pCallbackData->pMessage );
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        {
            AE_WARN( "%s", pCallbackData->pMessage );
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        {
            AE_ERROR( "%s", pCallbackData->pMessage );
            break;
        }        
        default:
            break;
    }

    
    AE_INFO_IF( type == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                "VK_PERFORMANCE %s", pCallbackData->pMessage );
    return VK_FALSE; // only used if testing validation layers
};
#endif

AEON_API Instance::Instance( Names extensions, Names layers )
{
    VkApplicationInfo appInfo
    { 
        VK_STRUCTURE_TYPE_APPLICATION_INFO,
        VK_NULL_HANDLE, //pNext
        "AER Application",
        VK_MAKE_API_VERSION( 1, 0, 1, 0 ),
        "AER",
        VK_MAKE_API_VERSION( 1, 0, 1, 0 ),
        VK_API_VERSION_1_2
    };

#ifndef NDEBUG
    VkDebugUtilsMessengerCreateInfoEXT debugInfo
    {
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        VK_NULL_HANDLE, // pNext
        VkDebugUtilsMessengerCreateFlagsEXT{ 0 },
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        debug_callback,
        VK_NULL_HANDLE  //pUserData
    };
#endif

    VkInstanceCreateInfo instanceInfo
    {
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
#ifndef NDEBUG
        static_cast<VkDebugUtilsMessengerCreateInfoEXT*>( &debugInfo ),
#else
        VK_NULL_HANDLE,
#endif
        VkInstanceCreateFlags{ 0 },
        &appInfo,
        static_cast<uint32_t>( layers.size() ),
        layers.empty() ? VK_NULL_HANDLE : layers.data(),
        static_cast<uint32_t>( extensions.size() ),
        extensions.empty() ? VK_NULL_HANDLE : extensions.data()
    };

    auto result = vkCreateInstance( &instanceInfo, VK_ALLOCATOR, &_instance );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create instance: %s", ResultMessage( result ) );
    gladLoaderLoadVulkan( _instance, nullptr, nullptr );

#ifndef NDEBUG
    GetProcAddr( CreateDebugUtilsMessenger,  "vkCreateDebugUtilsMessengerEXT"  );
    GetProcAddr( DestroyDebugUtilsMessenger, "vkDestroyDebugUtilsMessengerEXT" );

    if( CreateDebugUtilsMessenger )
    {
        result = CreateDebugUtilsMessenger
        ( 
            _instance, 
            &debugInfo, 
            VK_ALLOCATOR, 
            &_debug_messenger
        );
        AE_WARN_IF( result != VK_SUCCESS, "Failed to create debug messenger: %s", ResultMessage( result ) );
    }
#endif

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices( _instance, &device_count, VK_NULL_HANDLE );
    AE_FATAL_IF( device_count == 0, "Failed to find a GPU with Vulkan support" );

    Vector<VkPhysicalDevice> devices( device_count );
    vkEnumeratePhysicalDevices( _instance, &device_count, devices.data() );
    for( auto device : devices )
    {
        _physical_devices.emplace_back( new PhysicalDevice( this, device ) );
    }
}

ref_ptr<PhysicalDevice> Instance::physical_device( VkQueueFlags flags, Surface* surface, const PhysicalDeviceTypes& prefs )
{
    for( uint32_t i = 0; i <= prefs.size(); ++i )
    {
        for( auto& device : _physical_devices )
        {
            if( i < prefs.size() && device->properties().deviceType != prefs[i] ) continue;
            auto [ present_family, graphics_family ] = device->GetQueueFamilies( VK_QUEUE_GRAPHICS_BIT, surface );
            if( graphics_family >= 0 && present_family >= 0 ) return device;
        }
    }
    return{};
}

AEON_API Instance::~Instance()
{
#ifndef NDEBUG
    if( DestroyDebugUtilsMessenger )
    {
        DestroyDebugUtilsMessenger( _instance, _debug_messenger, VK_ALLOCATOR );
    }
#endif
    _physical_devices.clear();
    if( _instance ) vkDestroyInstance( _instance, VK_ALLOCATOR );
}

InstanceLayerProperties EnumerateInstanceLayerProperties()
{
    VkResult result{ VK_SUCCESS };
    uint32_t vk_layer_count{ 0 };
    result = vkEnumerateInstanceLayerProperties( &vk_layer_count, nullptr );
    AE_ERROR_IF( result != VK_SUCCESS, "Could not get instance layer count: %s", ResultMessage( result ) );

    Vector<VkLayerProperties> vk_layers(vk_layer_count);
    result = vkEnumerateInstanceLayerProperties( &vk_layer_count, vk_layers.data() );
    AE_ERROR_IF( result != VK_SUCCESS, "Could not get instance layer properties: %s", ResultMessage( result ) );

    return vk_layers;
}

InstanceExtensionProperties EnumerateInstanceExtensionProperties( Name layer_name = nullptr )
{
    VkResult result{ VK_SUCCESS };
    uint32_t count{ 0 };
    result = vkEnumerateInstanceExtensionProperties( layer_name, &count, nullptr );
    AE_ERROR_IF( result != VK_SUCCESS, "Could not get instance extension count: %s", ResultMessage( result ) );

    Vector<VkExtensionProperties> vk_extensions(count);
    result = vkEnumerateInstanceExtensionProperties( layer_name, &count, vk_extensions.data() );
    AE_ERROR_IF( result != VK_SUCCESS, "Could not get instance extension properties: %s", ResultMessage( result ) );

    return vk_extensions;
}

Names ValidateInstanceLayerNames( Names& names )
{
    if( names.empty() ) return names;

    auto instance_layers = EnumerateInstanceLayerProperties();

    std::set<std::string> available_layers{ instance_layers.begin(), instance_layers.end() };

    Names validated_names{ names.size() };
    for( const auto& requested : names )
    {
        if( available_layers.contains( requested ) ) validated_names.push_back( requested );
        else AE_WARN( "Invalid layer requested : %s", requested );
    }
    
    return validated_names;
}

std::string UnpackNames( const Names& names )
{
    std::stringstream unpacked;
    for( std::size_t i = 0; i < names.size(); )
    {
        unpacked << names[i] << ( ++i < names.size() ? ", " : "" );
    }
    return unpacked.str();
}

}
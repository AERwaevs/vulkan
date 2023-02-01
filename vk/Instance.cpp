#include <Platform/Vulkan/vk/Instance.h>
#include <Platform/Vulkan/vk/Surface.h>
#include <Platform/Vulkan/vk/PhysicalDevice.h>

namespace AEON::Graphics::vk
{

#ifdef AEON_DEBUG
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
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        {
            AE_WARN( "Vulkan %s", pCallbackData->pMessage );
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        {
            AE_ERROR( "Vulkan %s", pCallbackData->pMessage );
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

AEON_API Instance::Instance( Vector<const char*> instance_extensions, 
                             Vector<const char*> layers )
{
    VkResult result{ VK_SUCCESS };

    VkApplicationInfo appInfo
    { 
        VK_STRUCTURE_TYPE_APPLICATION_INFO,
        VK_NULL_HANDLE, //pNext
        "AEON Application",
        VK_MAKE_API_VERSION( 1, 0, 1, 0 ),
        "AEON",
        VK_MAKE_API_VERSION( 1, 0, 1, 0 ),
        VK_API_VERSION_1_2
    };

#ifdef AEON_DEBUG
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
#ifdef AEON_DEBUG
        static_cast<VkDebugUtilsMessengerCreateInfoEXT*>( &debugInfo ),
#else
        VK_NULL_HANDLE,
#endif
        VkInstanceCreateFlags{ 0 },
        &appInfo,
        static_cast<uint32_t>( layers.size() ),
        layers.empty() ? VK_NULL_HANDLE : layers.data(),
        static_cast<uint32_t>( instance_extensions.size() ),
        instance_extensions.empty() ? VK_NULL_HANDLE : instance_extensions.data()
    };

    result = vkCreateInstance( &instanceInfo, VK_ALLOCATOR, &m_instance );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create instance: vk%d", result );

#ifdef AEON_DEBUG
    GetProcAddr( CreateDebugUtilsMessenger,  "vkCreateDebugUtilsMessengerEXT"  );
    GetProcAddr( DestroyDebugUtilsMessenger, "vkDestroyDebugUtilsMessengerEXT" );

    if( CreateDebugUtilsMessenger )
    {
        result = CreateDebugUtilsMessenger
        ( 
            m_instance, 
            &debugInfo, 
            VK_ALLOCATOR, 
            &m_debug_messenger
        );
        AE_WARN_IF( result != VK_SUCCESS, "Failed to create debug messenger: vk%d", result );
    }
#endif

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices( m_instance, &device_count, VK_NULL_HANDLE );
    AE_FATAL_IF( device_count == 0, "Failed to find a GPU with Vulkan support" );

    Vector<VkPhysicalDevice> devices( device_count );
    vkEnumeratePhysicalDevices( m_instance, &device_count, devices.data() );
    for( auto device : devices )
    {
        m_physical_devices.emplace_back( new PhysicalDevice( this, device ) );
    }

    //TODO provide a sort function so we use best device as primary
    //std::sort( m_physical_devices.begin(), m_physical_devices.end(), 
    //[]( const Shared<PhysicalDevice>& a, const Shared<PhysicalDevice>& b )
    //{
    //    return a->Capability() > b->Capability();
    //} );
}

AEON_API Instance::~Instance()
{
#if AEON_DEBUG
    if( DestroyDebugUtilsMessenger )
    {
        DestroyDebugUtilsMessenger( m_instance, m_debug_messenger, VK_ALLOCATOR );
    }
#endif
    m_physical_devices.clear();
    if( m_instance ) vkDestroyInstance( m_instance, VK_ALLOCATOR );
}

}
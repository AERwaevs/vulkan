#include <Base/Base.h>

// only needed for getting HWND
//#include <Core/Application.h>
//#include <Graphics/Window.h>

#include <Platform/Vulkan/VulkanHelpers.h>
#include <Platform/Vulkan/vk/Instance.h>

#include <Vendor/glad/include/glad/vulkan.h>

#include <windows.h>

namespace AEON::Graphics::vk
{
    AEON_API Instance::Instance( Vector<const char*> instance_extensions, 
                                 Vector<const char*> layers )
    {
        VkResult result{ VK_SUCCESS };

        // create application info
        VkApplicationInfo appInfo
        { 
            VK_STRUCTURE_TYPE_APPLICATION_INFO,
            nullptr,    //pNext
            "AEON Application",
            VK_MAKE_API_VERSION( 1, 0, 1, 0 ),
            "AEON",
            VK_MAKE_API_VERSION( 1, 0, 1, 0 ),
            VK_API_VERSION_1_2
        };

        // create debug callback
        auto debug_callback = []
        (
            VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
            VkDebugUtilsMessageTypeFlagsEXT             type,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void*                                       pUserData
        ) -> VKAPI_ATTR VkBool32 VKAPI_CALL
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

        // create debug info
        VkDebugUtilsMessengerCreateInfoEXT debugInfo
        {
            VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            nullptr,    // pNext
            0,          // flags
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            debug_callback,
            nullptr     //pUserData
        };

        // create instance info
        VkInstanceCreateInfo instanceInfo
        {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            (VkDebugUtilsMessengerCreateInfoEXT*) &debugInfo,
            0,             // flags
            &appInfo,
            static_cast<uint32_t>( layers.size() ),
            layers.empty() ? nullptr : layers.data(),
            static_cast<uint32_t>( instance_extensions.size() ),
            instance_extensions.empty() ? nullptr : instance_extensions.data()
        };

        //* create instance
        result = vkCreateInstance( &instanceInfo, nullptr, &m_instance );
        AE_FATAL_IF( result != VK_SUCCESS, "Failed to create instance: vk%d", result );

        //* create debug messenger
        GetProcAddr( _vkCreateDebugUtilsMessengerEXT,  "vkCreateDebugUtilsMessengerEXT"  );
        GetProcAddr( _vkDestroyDebugUtilsMessengerEXT, "vkDestroyDebugUtilsMessengerEXT" );

        if( _vkCreateDebugUtilsMessengerEXT )
        {
            result = _vkCreateDebugUtilsMessengerEXT( m_instance, 
                                                      &debugInfo, 
                                                      nullptr, 
                                                      &m_vk_debug_messenger );
            AE_WARN_IF( result != VK_SUCCESS, "Failed to create debug messenger: vk%d", result );
        }

        //* pick physical device
        m_physical_device = pick_device( m_instance );

        //* create surface
        //VkWin32SurfaceCreateInfoKHR surfaceCreateInfo
        //{
        //    VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        //    nullptr,    // pNext
        //    0,          // flags
        //    GetModuleHandle( nullptr ),
        //    static_cast<HWND>( Application::Instance().GetWindows().front()->GetNativeWindow() )
        //};

        //result = vkCreateWin32SurfaceKHR( m_instance, &surfaceCreateInfo, nullptr, &m_surface );
        //AE_WARN_IF( result != VK_SUCCESS, "Failed to create surface: vk%d", result );

        // create logical device info
        QueueFamilyIndices indices{ find_queue_families( m_physical_device, m_surface ) };
        float priority{ 1.0f };

        Vector<VkDeviceQueueCreateInfo> queueInfos
        {
            {
                VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                nullptr, // pNext
                0,       // flags
                indices.graphics_family.value(),
                1,       // queue count
                &priority
            },
            {
                VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                nullptr, // pNext
                0,       // flags
                indices.present_family.value(),
                1,       // queue count
                &priority
            }
        };

        VkPhysicalDeviceFeatures device_features{}; // TODO specify features

        VkDeviceCreateInfo deviceCreateInfo
        {
            VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            nullptr,    // pNext
            0,          // flags
            static_cast<uint32_t>( queueInfos.size() ),
            queueInfos.empty() ? nullptr : queueInfos.data(),
            static_cast<uint32_t>( layers.size() ),
            layers.empty() ? nullptr : layers.data(),
            0,          // extension count
            nullptr,    // extension names
            &device_features
        };

        //* create logical device
        result = vkCreateDevice( m_physical_device, &deviceCreateInfo, nullptr, &m_device );
        AE_FATAL_IF( result != VK_SUCCESS, "Failed to create logical device: vk%d", result );

        vkGetDeviceQueue( m_device, indices.graphics_family.value(), 0, &m_queue_graphics );
        vkGetDeviceQueue( m_device, indices.present_family.value(),  0, &m_queue_present  );
    }

    AEON_API Instance::~Instance()
    {
        if( _vkDestroyDebugUtilsMessengerEXT )
        {
            _vkDestroyDebugUtilsMessengerEXT( m_instance, m_vk_debug_messenger, nullptr );
        }

        //vkDestroySurfaceKHR( m_instance, m_surface, nullptr );
        vkDestroyDevice( m_device, nullptr );
        vkDestroyInstance( m_instance, nullptr );

        gladLoaderUnloadVulkan();
    }
}
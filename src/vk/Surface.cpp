#include <Graphics/Window.h>

#include "Surface.h"

#if defined( AEON_PLATFORM_WINDOWS )
#   define  vkCreateSurfaceKHR vkCreateWin32SurfaceKHR
#elif defined( AEON_PLATFORM_ANDROID )
#   define  vkCreateSurfaceKHR vkCreateAndroidSurfaceKHR
#elif defined( AEON_PLATFORM_LINUX )
#   define  vkCreateSurfaceKHR vkCreateXcbSurfaceKHR
#endif

namespace AEON::Graphics::vk
{

#ifdef AEON_PLATFORM_WINDOWS
Surface::Surface( ref_ptr<Instance> instance, Window* window )
: _surface( VK_NULL_HANDLE ), _instance( instance )
{
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        VK_NULL_HANDLE, // pNext
        VkWin32SurfaceCreateFlagsKHR{ 0 },
        GetModuleHandle( nullptr ),
        window->native<HWND>()
    };
#elif AEON_PLATFORM_ANDROID
    VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
        VK_NULL_HANDLE, // pNext
        VkAndroidSurfaceCreateFlagsKHR{ 0 },
        window->native()
    };
#elif AEON_PLATFORM_LINUX
    VkXcbSurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
        VK_NULL_HANDLE, // pNext
        VkXcbSurfaceCreateFlagsKHR{ 0 },
        connection,
        window->native()
    };
#endif
    auto result = vkCreateSurfaceKHR( *instance, &surfaceCreateInfo, VK_ALLOCATOR, &_surface );
    AE_WARN_IF( result != VK_SUCCESS, "Failed to create surface: %s", ResultMessage( result ) );
}

Surface::~Surface()
{
    vkDestroySurfaceKHR( *_instance, _surface, nullptr );
}

}
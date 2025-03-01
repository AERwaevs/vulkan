#include <vk/Surface.h>

namespace aer::vk
{

#if defined( AER_PLATFORM_WINDOWS )
#include <windows.h>
#include <windowsx.h>

template<> Surface::Surface( Instance* instance, HWND window )
: _instance( instance ), _surface( VK_NULL_HANDLE )
{
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR, VK_NULL_HANDLE, VkWin32SurfaceCreateFlagsKHR{ 0 },
        GetModuleHandle( NULL ),
        window
    };
    auto result = vkCreateWin32SurfaceKHR( *instance, &surfaceCreateInfo, VK_ALLOCATOR, &_surface );
    CHECK_F( result == VK_SUCCESS, "Failed to create surface: %s", ResultMessage( result ) );
}

#elif defined( AER_PLATFORM_LINUX )
#include <vulkan/vulkan_xcb.h>

template<> Surface::Surface( Instance* instance, xcb_connection_t* connection, xcb_window_t window )
: _instance( instance ), _surface( VK_NULL_HANDLE )
{
    VkXcbSurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR, VK_NULL_HANDLE, VkXcbSurfaceCreateFlagsKHR{ 0 },
        connection,
        window
    };
    auto result = vkCreateXcbSurfaceKHR( *_instance, &surfaceCreateInfo, VK_ALLOCATOR, &_surface );
    CHECK_F( result == VK_SUCCESS, "Failed to create surface: %s", ResultMessage( result ) );
}

#elif defined( AER_PLATFORM_ANDROID )
    #pragma message( "Android surface not implemented" )
#endif
}
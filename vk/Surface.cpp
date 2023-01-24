#include <vk/Surface.h>

namespace AEON::Graphics::vk
{

Surface::Surface( Instance* instance, WINDOW_HANDLE window )
: m_surface( VK_NULL_HANDLE ), m_instance( instance )
{
#ifdef AEON_PLATFORM_WINDOWS
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        VK_NULL_HANDLE, // pNext
        VkWin32SurfaceCreateFlagsKHR{ 0 },
        GetModuleHandle( nullptr ),
        window
    };
#elif AEON_PLATFORM_ANDROID
    VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
        VK_NULL_HANDLE, // pNext
        VkAndroidSurfaceCreateFlagsKHR{ 0 },
        window
    };
#elif AEON_PLATFORM_LINUX
    VkXcbSurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
        VK_NULL_HANDLE, // pNext
        VkXcbSurfaceCreateFlagsKHR{ 0 },
        connection,
        window
    };
#endif
    auto result = vkCreateSurfaceKHR
    (
        *instance,
        &surfaceCreateInfo,
        VK_ALLOCATOR,
        &m_surface
    );

    AE_WARN_IF( result != VK_SUCCESS, "Failed to create surface: vk%d", result );
}

Surface::~Surface()
{
    vkDestroySurfaceKHR( *m_instance, m_surface, nullptr );
}
}
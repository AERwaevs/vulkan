#include <vk/Surface.h>

namespace aer::vk
{

#if defined( AER_PLATFORM_WINDOWS )

template<> ref_ptr<Surface>
Surface::create( ref_ptr<Instance> instance, Window_t window )
{
    return ref_ptr<Win32Surface>( new Win32Surface( instance, window ) );
}

Win32Surface::Win32Surface( ref_ptr<Instance> instance, Window_t window )
: inherit( instance )
{
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        VK_NULL_HANDLE, // pNext
        VkWin32SurfaceCreateFlagsKHR{ 0 },
        GetModuleHandle( nullptr ),
        window
    };
    auto result = vkCreateWin32SurfaceKHR( *instance, &surfaceCreateInfo, VK_ALLOCATOR, &_surface );

#elif defined( AER_PLATFORM_ANDROID )

template<> ref_ptr<Surface>
Surface::create( ref_ptr<Instance> instance, Window_t window )
{
    return ref_ptr<AndroidSurface>( new AndroidSurface( instance, window ) );
}

AndroidSurface::AndroidSurface( ref_ptr<Instance> instance, Window_t window )
: inherit( instance )
{
    VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
        VK_NULL_HANDLE, // pNext
        VkAndroidSurfaceCreateFlagsKHR{ 0 },
        window
    };
    auto result = vkCreateAndroidSurfaceKHR( *instance, &surfaceCreateInfo, VK_ALLOCATOR, &_surface );

#elif defined( AER_PLATFORM_LINUX )
#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>

template<> Surface::Surface( ref_ptr<Instance> instance, xcb_connection_t* connection, xcb_window_t window )
: _instance( instance ), _surface( VK_NULL_HANDLE )
{
    VkXcbSurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR, VK_NULL_HANDLE, VkXcbSurfaceCreateFlagsKHR{ 0 },
        connection,
        window
    };
    auto result = vkCreateXcbSurfaceKHR( *_instance, &surfaceCreateInfo, VK_ALLOCATOR, &_surface );
    AE_WARN_IF( result != VK_SUCCESS, "Failed to create surface: %s", ResultMessage( result ) );
}
#endif

Surface::~Surface() noexcept
{
    if( _surface ) vkDestroySurfaceKHR( *_instance, _surface, VK_ALLOCATOR );
}
}
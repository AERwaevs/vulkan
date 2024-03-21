#pragma once

#include "vk.h"
#include "Instance.h"

namespace aer::gfx::vk
{

struct Surface : public Object
{
    template< typename... Args > static ref_ptr<Surface> create( Args... );
    Surface( ref_ptr<Instance> instance ) : _instance( instance ), _surface( VK_NULL_HANDLE ) {};

    operator VkSurfaceKHR() const { return _surface; }
    auto     vk()           const { return _surface; }
protected:
    virtual ~Surface();
    VkSurfaceKHR        _surface;
    ref_ptr<Instance>   _instance;
};

#if defined( AEON_PLATFORM_WINDOWS )

using Window_t = HWND;
struct Win32Surface : public Surface    { Win32Surface( ref_ptr<Instance>, Window_t ); };

#elif defined( AEON_PLATFORM_ANDROID )

using Window_t = ANativeWindow*;
struct AndroidSurface : public Surface  { AndroidSurface( ref_ptr<Instance>, Window_t ); };

#elif defined( AEON_PLATFORM_LINUX )
#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>

struct Window_t : private std::pair<xcb_connection_t*, xcb_window_t>
{
    auto connection(){ return first; };
    auto window()    { return second; };
};

struct XCBSurface : public Surface    { XCBSurface( ref_ptr<Instance>, Window_t ); };

#endif

}
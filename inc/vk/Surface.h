#pragma once

#include "vk.h"
#include "Instance.h"

#if defined( AER_PLATFORM_WINDOWS )

#elif defined( AER_PLATFORM_ANDROID )

#elif defined( AER_PLATFORM_LINUX )
#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>
#endif

namespace aer::vk
{

struct Surface : public Object
{
    //template< typename... Args > static ref_ptr<Surface> create( Args... );
    //Surface( ref_ptr<Instance> instance ) : _instance( instance ), _surface( VK_NULL_HANDLE ) {};
    
    template< typename... Args > Surface( ref_ptr<Instance>, Args... );

#if defined( AER_PLATFORM_WINDOWS )

#elif defined( AER_PLATFORM_ANDROID )

#elif defined( AER_PLATFORM_LINUX )
    Surface( ref_ptr<Instance>, xcb_connection_t* connection, xcb_window_t window );
#endif

    ~Surface() noexcept;

    operator VkSurfaceKHR() const { return _surface; }
    auto     vk()           const { return _surface; }
    VkSurfaceKHR        _surface;
    ref_ptr<Instance>   _instance;
};

}
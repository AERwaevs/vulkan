#pragma once

#include <Vendor/glad/include/glad/vulkan.h>

#include <vk/Instance.h>

#if defined( AEON_PLATFORM_WINDOWS )
#   include "windows.h"
#   define  WINDOW_HANDLE HWND
#elif defined( AEON_PLATFORM_ANDROID )
#   define  WINDOW_HANDLE ANativeWindow*
#elif defined( AEON_PLATFORM_LINUX )
#   define  WINDOW_HANDLE xcb_connection_t* connection, xcb_window_t
#endif

namespace AEON::Graphics::vk
{

class AEON_DLL Surface : public Inherit< Object, Surface >
{
public:
    Surface( Instance* instance, WINDOW_HANDLE window );
    operator VkSurfaceKHR() const { return m_surface; }

protected:
    ~Surface();
private:
    VkSurfaceKHR        m_surface;
    Shared<Instance>    m_instance;
};

}
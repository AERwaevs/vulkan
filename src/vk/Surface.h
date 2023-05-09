#pragma once

#include <glad/vulkan.h>

#include "Instance.h"

namespace AEON::Graphics{ class Window; }

namespace AEON::Graphics::vk
{

class Surface : public virtual Object, public Implements< Surface, ICreate >
{
public:
#if defined( AEON_PLATFORM_WINDOWS )
    Surface( ref_ptr<Instance> instance, HWND window );
#elif defined( AEON_PLATFORM_ANDROID )
    Surface( ref_ptr<Instance> instance, ref_ptr<Window> window );
#elif defined( AEON_PLATFORM_LINUX )
    Surface( ref_ptr<Instance> instance, ref_ptr<Window> window );
#endif
    operator VkSurfaceKHR() const { return _surface; }

protected:
    ~Surface();
private:
    VkSurfaceKHR        _surface;
    ref_ptr<Instance>    _instance;
};

}
#pragma once

#include <Graphics/Window.h>

#include <glad/vulkan.h>

#include "Instance.h"

namespace AEON::Graphics::vk
{

class Surface : public virtual Object, public Implements< Surface, ICreate >
{
public:
    Surface( ref_ptr<Instance> instance, Window* window );
    operator VkSurfaceKHR() const { return _surface; }

protected:
    ~Surface();
private:
    VkSurfaceKHR        _surface;
    ref_ptr<Instance>    _instance;
};

#if defined( AEON_PLATFORM_WINDOWS )
class Win32Surface : public Surface
{
public:
    Win32Surface( ref_ptr<Instance> instance, Window* window );
};
#elif defined( AEON_PLATFORM_ANDROID )
    Surface( ref_ptr<Instance> instance, ref_ptr<Window> window );
#elif defined( AEON_PLATFORM_LINUX )
    Surface( ref_ptr<Instance> instance, ref_ptr<Window> window );
#endif

}
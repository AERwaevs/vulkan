#pragma once

#include <glad/vulkan.h>

#include "Instance.h"

namespace AEON::Graphics{ class Window; }

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

}
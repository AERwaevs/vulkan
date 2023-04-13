#pragma once

#include <glad/vulkan.h>

#include "Instance.h"

namespace AEON::Graphics{ class Window; }

namespace AEON::Graphics::vk
{

class Surface : public virtual Object, public Implements< Surface, ICreatable >
{
public:
    Surface( Shared<Instance> instance, Window* window );
    operator VkSurfaceKHR() const { return _surface; }

protected:
    ~Surface();
private:
    VkSurfaceKHR        _surface;
    Shared<Instance>    _instance;
};

}
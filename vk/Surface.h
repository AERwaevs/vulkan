#pragma once

#include <Core/Window.h>
#include <glad/vulkan.h>

#include "Instance.h"

namespace AEON::Graphics::vk
{

class Surface : public Object< Surface >
{
public:
    Surface( Shared<Instance> instance, Window* window );
    operator VkSurfaceKHR() const { return m_surface; }

protected:
    ~Surface();
private:
    VkSurfaceKHR        m_surface;
    Shared<Instance>    m_instance;
};

}
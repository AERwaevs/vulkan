#pragma once

#include <Core/Window.h>
#include <Vendor/glad/include/glad/vulkan.h>

#include <vk/Instance.h>

namespace AEON::Graphics::vk
{

class AEON_DLL Surface : public Object
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
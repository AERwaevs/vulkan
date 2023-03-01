#pragma once

#include <Base/Base.h>

#include <Graphics/Renderer.h>

#include "vk/Instance.h"

namespace AEON::Graphics
{
    class AEON_DLL VulkanRenderer : public Renderer
    {
    public:
        VulkanRenderer();
        Shared<vk::Instance> vkinstance() const { return m_instance; }
    protected:
        virtual         ~VulkanRenderer();
    private:
        Shared<vk::Instance>        m_instance;
    };
}
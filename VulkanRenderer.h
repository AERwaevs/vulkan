#pragma once

#include <Base/Base.h>

#include <Graphics/Renderer.h>

#include <Platform/Vulkan/vk/Instance.h>

namespace AEON::Graphics
{
    class AEON_DLL VulkanRenderer : public Inherit< Renderer, VulkanRenderer >
    {
    public:
        VulkanRenderer();
    protected:
        virtual ~VulkanRenderer();
    private:
        Shared<vk::Instance>        m_instance;
    };
}
#pragma once

#include <Graphics/Renderer.h>

#include "vk/vk.h"

namespace aer::gfx
{
    class VulkanRenderer : public inherit< VulkanRenderer, Renderer >
    {
    public:
        VulkanRenderer();
    protected:
        virtual         ~VulkanRenderer() noexcept = default;
    };

}
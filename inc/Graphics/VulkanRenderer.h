#pragma once

#include <Graphics/Renderer.h>

#include "vk/vk.h"

namespace aer::gfx
{
    class VulkanRenderer : public Renderer
    {
    public:
        VulkanRenderer();
        virtual         ~VulkanRenderer() noexcept = default;
    };

}
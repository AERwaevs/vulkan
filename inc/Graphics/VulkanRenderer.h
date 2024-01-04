#pragma once

#include <Graphics/Renderer.h>

#include "vk/vk.h"

namespace aer::gfx
{
    class VulkanRenderer : public Renderer, public Interfaces< VulkanRenderer, ICreateIf >
    {
    public:
        VulkanRenderer();
    protected:
        virtual         ~VulkanRenderer() = default;
    };

}
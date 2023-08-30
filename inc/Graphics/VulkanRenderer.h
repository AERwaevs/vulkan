#pragma once

#include <Graphics/Renderer.h>

#include "vk/vk.h"

namespace aer::gfx
{
    inline bool VulkanSupported()
    {
        static bool loaded( false );
        if( loaded ) return true;
        static auto version( gladLoaderLoadVulkan( nullptr, nullptr, nullptr ) );
        return loaded = ( version >= AEON_VK_VERSION );
    }

    class VulkanRenderer : public Renderer, public Interfaces< VulkanRenderer, ICreateIf >
    {
    public:
        VulkanRenderer();
    protected:
        virtual         ~VulkanRenderer() = default;
    };

}
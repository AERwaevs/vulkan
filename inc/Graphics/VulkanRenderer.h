#pragma once

#include <Graphics/Renderer.h>

#include <glad/vulkan.h>

#include <vk/Instance.h>

#define AEON_VK_VERSION GLAD_MAKE_VERSION( 1, 2 )

namespace aer::gfx
{
    inline bool VulkanSupported()
    {
        static bool loaded( false );
        if( loaded ) return true;
        int version = gladLoaderLoadVulkan( nullptr, nullptr, nullptr );
        AE_INFO
        (
            "Vulkan version: %d.%d", 
            GLAD_VERSION_MAJOR(version),
            GLAD_VERSION_MINOR(version)
        );
        return loaded = ( version >= AEON_VK_VERSION );
    }

    class VulkanRenderer : public Renderer, public Interfaces< VulkanRenderer, ICreateIf >
    {
    public:
        VulkanRenderer();
        ref_ptr<vk::Instance> vkinstance() const { return _vkinstance; }
    protected:
        virtual         ~VulkanRenderer();
    private:
        ref_ptr<vk::Instance>        _vkinstance;
    };

}
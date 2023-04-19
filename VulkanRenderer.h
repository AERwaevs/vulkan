#pragma once

#include <Base/Base.h>

#include <Graphics/Renderer.h>

#include "../../Vendor/glad/include/glad/vulkan.h"

#include "vk/Instance.h"

#define AEON_VK_VERSION GLAD_MAKE_VERSION( 1, 2 )

namespace AEON::Graphics
{
    inline bool VulkanSupported()
    {
        int version = gladLoaderLoadVulkan( nullptr, nullptr, nullptr );
        AE_INFO( "Vulkan version: %d.%d", 
                    GLAD_VERSION_MAJOR(version),
                    GLAD_VERSION_MINOR(version) );
        return version >= AEON_VK_VERSION;
    }

    class VulkanRenderer : public Renderer, public Implements< VulkanRenderer, ICreateIf >
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
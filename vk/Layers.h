#pragma once

#include <Base/Base.h>

#include <Base/Log.h>

#include <Vendor/glad/include/glad/vulkan.h>

namespace AEON::Graphics::vk
{
    inline Vector<VkLayerProperties> EnumerateInstanceLayerProperties()
    {
        VkResult result( VK_SUCCESS );
        uint32_t vk_layer_count( 0 );
        result = vkEnumerateInstanceLayerProperties( &vk_layer_count, nullptr );
        AE_ERROR_IF( result != VK_SUCCESS, 
                     "Failed to enumerate instance extension count: vk%d", result );

        Vector<VkLayerProperties> vk_layers(vk_layer_count);
        result = vkEnumerateInstanceLayerProperties( &vk_layer_count, vk_layers.data() );
        AE_ERROR_IF( result != VK_SUCCESS, 
                     "Failed to enumerate instance extension properties: vk%d", result );

        return vk_layers;
    }

    inline Vector<const char*> GetRequiredLayers()
    {
        Vector<const char*> layers( {
#ifndef AEON_RELEASE
            "VK_LAYER_KHRONOS_validation"
#endif
        } );

        // TODO validate requested layers

        return layers;
    }
}
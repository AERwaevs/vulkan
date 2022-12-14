#pragma once

#include <optional>

#include <Vendor/glad/include/glad/vulkan.h>

#define VK_EXT_FUNC( function, ... ) \
        AEON::Graphics::vk::run_extension_function<PFN_##function>( m_instance, #function, __VA_ARGS__ )

namespace AEON::Graphics::vk
{
    // TODO make generic so can be used for compute, memory, etc
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        bool complete() const
        {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    template< typename F, typename... Args >
    VkResult run_extension_function(
        VkInstance  instance,
        const char* name,
        Args&&...   arguments )
    {
        auto func = reinterpret_cast<F>( vkGetInstanceProcAddr( instance, name ) );
        if( func == nullptr ) return VK_ERROR_EXTENSION_NOT_PRESENT;

        using return_type = typename decltype( std::function{ std::declval<F>() } )::result_type;
        if constexpr( std::is_same< return_type, void >() )
        {
            func( instance, arguments... );
            return VK_SUCCESS;
        }
        else return func( instance, arguments... );
    }

    bool device_supported( VkPhysicalDevice device );
    int device_capability( VkPhysicalDevice device );

    VkPhysicalDevice pick_device( VkInstance instance );

    QueueFamilyIndices find_queue_families( VkPhysicalDevice device, VkSurfaceKHR surface = VK_NULL_HANDLE );
}

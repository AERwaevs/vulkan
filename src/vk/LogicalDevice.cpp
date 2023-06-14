#include "LogicalDevice.h"
#include "Instance.h"

namespace aer::gfx::vk
{

//? maybe remove requirement for a surface? this will allow for compute-only devices
Device::Device( ref_ptr<PhysicalDevice> physical_device, ref_ptr<Surface> surface,
                const QueueSettings& queue_settings )
: _instance{ physical_device->instance() }, _physical_device( physical_device )
{
    const auto& layers( Device::RequiredLayers );
    const auto& extensions( Device::RequiredExtensions );
    const auto  queue_priority( 1.0f );

    Vector<VkDeviceQueueCreateInfo> queue_infos;
    for( auto& setting : queue_settings )
    {
        if( setting.queue_family_index < 0 ) continue;

        const auto& setting_index( static_cast<uint32_t>( setting.queue_family_index ) );
        const auto& queue_count( static_cast<uint32_t>( setting.queue_priorities.size() ) );

        const auto unique([&]()
        {
            for( auto& existing : queue_infos )
            {
                if( existing.queueFamilyIndex == setting_index ) return false;
            }
            return true;
        }());
        if( !unique ) continue;

        queue_infos.push_back
        ({
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            VK_NULL_HANDLE, // pNext
            VkDeviceQueueCreateFlags{ 0 },
            setting_index,
            setting.queue_priorities.empty() ? 1 : queue_count,
            setting.queue_priorities.empty() ? &queue_priority : setting.queue_priorities.data()
        });
    };

    VkDeviceCreateInfo deviceCreateInfo
    {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        VK_NULL_HANDLE,             // pNext
        VkDeviceCreateFlags{ 0 },
        static_cast<uint32_t>( queue_infos.size() ),
        queue_infos.empty() ? VK_NULL_HANDLE : queue_infos.data(),
        static_cast<uint32_t>( layers.size() ),
        layers.empty() ? VK_NULL_HANDLE : layers.data(),
        static_cast<uint32_t>( extensions.size() ),
        extensions.empty() ? VK_NULL_HANDLE : extensions.data(),
        &physical_device->features()
    };

    //* create logical device
    auto result = vkCreateDevice( *_physical_device, &deviceCreateInfo, VK_ALLOCATOR, &_device );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create logical device: %s", ResultMessage( result ) );

    gladLoaderLoadVulkan( *_instance, *_physical_device, _device );
    vkGetDeviceQueue( _device, queue_infos[0].queueFamilyIndex, 0, &_queue_present  );
    vkGetDeviceQueue( _device, queue_infos[0].queueFamilyIndex, 0, &_queue_graphics  );

#ifdef AEON_DEBUG
    AE_INFO_IF
    (
        enable_ctor_logging,
        "VkDevice\n"
        "{\n"
        "    Queue count             = %zu\n"
        "    Queues                  = %s\n"
        "    Enabled layer count     = %zu\n"
        "    Enabled layer names     = %s\n"
        "    Enabled extension count = %zu\n"
        "    Enabled extension names = %s\n"
        "}\n",
        queue_infos.size(),
        [&]()
        {
            std::stringstream queues;
            for( auto info : queue_infos )
            {
                queues << info.queueFamilyIndex << " ";
            }
            return queues.str();
        }().c_str(), 
        layers.size(), UnpackNames( layers ).c_str(),
        extensions.size(), UnpackNames( extensions ).c_str()
    );
#endif
}

Device::~Device()
{
    AE_INFO_IF( enable_dtor_logging, "Destroying VkDevice" );
    vkDestroyDevice( _device, VK_ALLOCATOR );
}

} // namespace aer::gfx::vk
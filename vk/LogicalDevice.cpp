#include <vk/LogicalDevice.h>
#include <vk/Layers.h>
#include <vk/Extensions.h>

namespace AEON::Graphics::vk
{

//? maybe remove requirement for a surface? this will allow for compute-only devices
Device::Device( Shared<PhysicalDevice> physical_device, Shared<Surface> surface )
: m_instance{ physical_device->instance() }, m_physical_device( physical_device )
{
    VkResult result{ VK_SUCCESS };

    //? might require actual priorities later as optimization step?
    float priority{ 1.0f };
    const auto& layers{ GetRequiredLayers() };
    const auto& extensions{ GetRequiredExtensions() };
    const auto& graphics_family_index
    {
        physical_device->GetQueueFamily( VK_QUEUE_GRAPHICS_BIT )
    };
    const auto& present_family_index
    {
        physical_device->GetQueueFamily( VK_QUEUE_GRAPHICS_BIT, surface.get() )
    };

    //? maybe uncouple hard-coded queue infos?
    Vector<VkDeviceQueueCreateInfo> queueInfos
    {
        {
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            VK_NULL_HANDLE, // pNext
            VkDeviceQueueCreateFlags{ 0 },
            graphics_family_index,
            1,              // queue count
            &priority
        },
        {
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            VK_NULL_HANDLE, // pNext
            VkDeviceQueueCreateFlags{ 0 },
            present_family_index,
            1,              // queue count
            &priority
        }
    };

    VkDeviceCreateInfo deviceCreateInfo
    {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        VK_NULL_HANDLE,             // pNext
        VkDeviceCreateFlags{ 0 },
        static_cast<uint32_t>( queueInfos.size() ),
        queueInfos.empty() ? VK_NULL_HANDLE : queueInfos.data(),
        static_cast<uint32_t>( layers.size() ),
        layers.empty() ? VK_NULL_HANDLE : layers.data(),
        static_cast<uint32_t>( extensions.size() ),
        extensions.empty() ? VK_NULL_HANDLE : extensions.data(),
        &physical_device->features()
    };

    //* create logical device
    result = vkCreateDevice( *m_physical_device, &deviceCreateInfo, VK_ALLOCATOR, &m_device );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create logical device: vk%d", result );

    vkGetDeviceQueue( m_device, graphics_family_index, 0, &m_queue_graphics );
    vkGetDeviceQueue( m_device, present_family_index,  0, &m_queue_present  );
}

Device::~Device()
{
    vkDestroyDevice( m_device, VK_ALLOCATOR );
}

}
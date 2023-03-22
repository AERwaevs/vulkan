#pragma once

#include <glad/vulkan.h>

#include "Instance.h"
#include "Surface.h"

namespace AEON::Graphics::vk
{

class PhysicalDevice : public Object< PhysicalDevice >
{
public:
                PhysicalDevice( Instance* instance, VkPhysicalDevice device );
    operator    VkPhysicalDevice() const { return m_device; }


    bool        Supported() const;

    auto                            EnumerateExtensionProperties( Name layer_name = nullptr ) const;
    uint32_t                        GetQueueFamily( VkQueueFlags flags ) const;
    std::pair<uint32_t, uint32_t>   GetQueueFamilies( VkQueueFlags flags, Surface* surface ) const;

          Observer<Instance>            instance()   const { return m_instance; }
    const VkPhysicalDeviceFeatures&     features()   const { return m_features; }
    const VkPhysicalDeviceProperties&   properties() const { return m_properties; }

protected:
    virtual ~PhysicalDevice(){};


private:
    VkPhysicalDevice            m_device;
    VkPhysicalDeviceFeatures    m_features;
    VkPhysicalDeviceProperties  m_properties;

    using QueueFamilyProperties = Vector<VkQueueFamilyProperties>;
    QueueFamilyProperties       m_queue_families;

    PFN_vkGetPhysicalDeviceFeatures2   GetPhysicalDeviceFeatures2   = nullptr;
    PFN_vkGetPhysicalDeviceProperties2 GetPhysicalDeviceProperties2 = nullptr;

    Observer<Instance>          m_instance;

public:
    static const Names RequiredExtensions() { return
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    }; }

};

}
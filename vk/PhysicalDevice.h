#pragma once

#include <Vendor/glad/include/glad/vulkan.h>

#include <vk/Instance.h>
#include <vk/Surface.h>

namespace AEON::Graphics::vk
{

class AEON_DLL PhysicalDevice : public Inherit< Object, PhysicalDevice >
{
public:
                PhysicalDevice( Instance* instance, VkPhysicalDevice device );
    operator    VkPhysicalDevice() const { return m_device; }


    bool        Supported() const;
    uint32_t    Capability() const;

    uint32_t    GetQueueFamily( VkQueueFlags flags ) const;
    uint32_t    GetQueueFamily( VkQueueFlags flags, Surface* surface ) const;

          Observer<Instance>            GetInstance()   const { return m_instance; }
    const VkPhysicalDeviceFeatures&     GetFeatures()   const { return m_features; }
    const VkPhysicalDeviceProperties&   GetProperties() const { return m_properties; }

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
};

}
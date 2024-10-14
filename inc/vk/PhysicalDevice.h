#pragma once

#include "vk.h"
#include "Instance.h"
#include "Surface.h"

namespace aer::gfx::vk
{

struct PhysicalDevice : public inherit< PhysicalDevice, Object >
{
                PhysicalDevice( Instance* instance, VkPhysicalDevice device );
    operator    VkPhysicalDevice() const { return _device; }
    virtual ~PhysicalDevice() noexcept = default;

    bool        Supported() const;

    Vector<VkExtensionProperties> EnumerateExtensionProperties( Name layer_name = nullptr ) const;
    int                 GetQueueFamily( VkQueueFlags flags ) const;
    std::pair<int, int> GetQueueFamilies( VkQueueFlags flags, Surface* surface ) const;

          spy_ptr<Instance>            instance()   const { return _instance; }
    const VkPhysicalDeviceFeatures&     features()   const { return _features; }
    const VkPhysicalDeviceProperties&   properties() const { return _properties; }

private:
    VkPhysicalDevice            _device;
    VkPhysicalDeviceFeatures    _features;
    VkPhysicalDeviceProperties  _properties;

    using QueueFamilyProperties = Vector<VkQueueFamilyProperties>;
    QueueFamilyProperties       _queue_families;

    PFN_vkGetPhysicalDeviceFeatures2   GetPhysicalDeviceFeatures2   = nullptr;
    PFN_vkGetPhysicalDeviceProperties2 GetPhysicalDeviceProperties2 = nullptr;

    spy_ptr<Instance>          _instance;

public:
    static inline const Names RequiredExtensions
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

};

}
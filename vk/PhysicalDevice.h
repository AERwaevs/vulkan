#pragma once

#include <glad/vulkan.h>

#include "Instance.h"
#include "Surface.h"

namespace AEON::Graphics::vk
{

class PhysicalDevice : public Object
{
public:
                PhysicalDevice( Instance* instance, VkPhysicalDevice device );
    operator    VkPhysicalDevice() const { return _device; }


    bool        Supported() const;

    auto                            EnumerateExtensionProperties( Name layer_name = nullptr ) const;
    uint32_t                        GetQueueFamily( VkQueueFlags flags ) const;
    std::pair<uint32_t, uint32_t>   GetQueueFamilies( VkQueueFlags flags, Surface* surface ) const;

          Observer<Instance>            instance()   const { return _instance; }
    const VkPhysicalDeviceFeatures&     features()   const { return _features; }
    const VkPhysicalDeviceProperties&   properties() const { return _properties; }

protected:
    virtual ~PhysicalDevice(){};


private:
    VkPhysicalDevice            _device;
    VkPhysicalDeviceFeatures    _features;
    VkPhysicalDeviceProperties  _properties;

    using QueueFamilyProperties = Vector<VkQueueFamilyProperties>;
    QueueFamilyProperties       _queue_families;

    PFN_vkGetPhysicalDeviceFeatures2   GetPhysicalDeviceFeatures2   = nullptr;
    PFN_vkGetPhysicalDeviceProperties2 GetPhysicalDeviceProperties2 = nullptr;

    Observer<Instance>          _instance;

public:
    static const Names RequiredExtensions() { return
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    }; }

};

}
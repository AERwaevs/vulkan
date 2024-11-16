#pragma once

#include "../vk.h"
#include "../LogicalDevice.h"

namespace aer::vk
{

struct Image : public Object
{
    VkImageCreateFlags      flags       = 0;
    VkImageType             imageType   = VK_IMAGE_TYPE_2D;
    VkFormat                format      = VK_FORMAT_UNDEFINED;
    VkExtent3D              extent      = { 0, 0, 0 };
    uint32_t                mipLevels   = 0;
    uint32_t                arrayLayers = 0;
    VkSampleCountFlagBits   samples     = VK_SAMPLE_COUNT_1_BIT;
    VkImageTiling           tiling      = VK_IMAGE_TILING_OPTIMAL;
    VkImageUsageFlags       usage       = 0;
    VkSharingMode           sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    std::vector<uint32_t>   queueFamilyIndices;
    VkImageLayout           layout      = VK_IMAGE_LAYOUT_UNDEFINED;

    // creates a new image wrapper for specified VkImage
    Image( VkImage image, Device* device );
    ~Image() noexcept;
    operator VkImage() const { return _image; };
    
    void Compile( Device* device );


    ref_ptr<Device> _device;
    VkImage         _image;
};
using Images = std::vector<ref_ptr<Image>>;

} // namespace aer::vk
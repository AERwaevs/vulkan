#pragma once

#include <Base/Base.h>

#include <glad/vulkan.h>

#include "LogicalDevice.h"

namespace aer::Graphics::vk
{

class Image : public virtual Object, public Interfaces< Image, ICreate >
{
public:
    VkImageCreateFlags      flags       = 0;
    VkImageType             imageType   = VK_IMAGE_TYPE_2D;
    VkFormat                format      = VK_FORMAT_UNDEFINED;
    VkExtent3D              extent      = { 0, 0, 0 };
    uint32_t                mipLevels   = 0;
    uint32_t                arrayLayers = 0;
    VkSampleCountFlagBits   samples     = VK_SAMPLE_COUNT_1_BIT;
    VkImageTiling           tiling      = VK_IMAGE_TILING_OPTIMAL;
    VkImageUsageFlags       usage       = 0;
    VkSharingMode           shareMode   = VK_SHARING_MODE_EXCLUSIVE;
    std::vector<uint32_t>   queueFamilyIndices;
    VkImageLayout           layout      = VK_IMAGE_LAYOUT_UNDEFINED;
public:
    Image( VkImage image, Device* device );
    operator VkImage() const { return _image; };
public:
    void Compile( Device* device = nullptr );
protected:
    virtual ~Image();
protected:
    ref_ptr<Device> _device;
    VkImage         _image;
};
using Images = std::vector<ref_ptr<Image>>;

}
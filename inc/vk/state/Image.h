#pragma once

#include "../vk.h"
#include "../LogicalDevice.h"

namespace aer::gfx::vk
{

struct Image : public virtual Object, public Interfaces< Image, ICreate >
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

    Image( VkImage image, Device* device );
    operator VkImage() const { return _image; };
    
    void Compile( Device* device );

protected:
    virtual ~Image();

    ref_ptr<Device> _device;
    VkImage         _image;
};
using Images = std::vector<ref_ptr<Image>>;

} // namespace aer::gfx::vk
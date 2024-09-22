#pragma once

#include "../vk.h"
#include "../LogicalDevice.h"

#include "Image.h"

namespace aer::gfx::vk
{

class ImageView : public inherit< ImageView, Object >
{
public:
    VkImageViewCreateFlags  flags        = 0;
    VkImageViewType         viewType    = VK_IMAGE_VIEW_TYPE_2D;
    VkFormat                format       = VK_FORMAT_UNDEFINED;
    VkImageSubresourceRange subresourceRange;
    VkComponentMapping      components
    {
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY
    };
    ref_ptr<Image> image;
public:
    ImageView( ref_ptr<Image> in_image = {}, VkImageAspectFlags = VK_IMAGE_ASPECT_NONE_KHR );

    operator VkImageView()  const { return _imageView; }
    auto vk()               const { return _imageView; }
public:
    void Compile( Device* device );
protected:
    virtual ~ImageView();
protected:
    ref_ptr<Device>         _device;
    VkImageView             _imageView = VK_NULL_HANDLE;
};
using ImageViews = std::vector<ref_ptr<ImageView>>;

}
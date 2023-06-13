#pragma once

#include <Base/Base.h>

#include <glad/vulkan.h>

#include "../Instance.h"
#include "../LogicalDevice.h"

#include "Image.h"

namespace aer::Graphics::vk
{

class ImageView : public virtual Object, public Interfaces< ImageView, ICreate >
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
    ImageView( ref_ptr<Image> in_image = {} );
public:
    void Compile( Device* device );
protected:
    virtual ~ImageView();
protected:
    ref_ptr<Device>         _device;
    VkImageView             _image_view = VK_NULL_HANDLE;
};
using ImageViews = std::vector<ref_ptr<ImageView>>;

}
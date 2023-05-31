#include "ImageView.h"
#include "LogicalDevice.h"

namespace AEON::Graphics::vk
{

ImageView::ImageView( ref_ptr<Image> in_image )
: image( in_image )
{
    if( image )
    {
        viewType                        = VK_IMAGE_VIEW_TYPE_2D; // TODO - handle other image types
        format                           = image->format;
        subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT; //TODO - get correct aspect flags
        subresourceRange.baseMipLevel   = 0;
        subresourceRange.levelCount     = image->mip_levels;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount     = image->arr_layers;
    }
}

void ImageView::Compile( Device* device )
{
    VkImageViewCreateInfo create_info
    {
        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        VK_NULL_HANDLE,
        VkImageViewCreateFlags{ 0 },
        image ? *image : VK_NULL_HANDLE,
        VK_IMAGE_VIEW_TYPE_2D,
        format,
        components,
        subresourceRange
    };

    _device = device;

    if( image )
    {
        image->Compile( device );
    }

    auto result = vkCreateImageView( *device, &create_info, VK_ALLOCATOR, &_image_view );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create vkImageView" );
}

ImageView::~ImageView()
{
    if( _image_view )
    {
        vkDestroyImageView( *_device, _image_view, VK_ALLOCATOR );
        _image_view = VK_NULL_HANDLE;
        _device = {};
    }
}

} // namespace AEON::Graphics::vk
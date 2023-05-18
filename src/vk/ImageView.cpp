#include "ImageView.h"
#include "LogicalDevice.h"

namespace AEON::Graphics::vk
{

ImageView::ImageView( ref_ptr<Image> in_image )
: image( in_image )
{
    if( image )
    {
        view_type                        = VK_IMAGE_VIEW_TYPE_2D; // TODO - handle other image types
        format                           = image->format;
        subresource_range.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT; //TODO - get correct aspect flags
        subresource_range.baseMipLevel   = 0;
        subresource_range.levelCount     = image->mip_levels;
        subresource_range.baseArrayLayer = 0;
        subresource_range.layerCount     = image->arr_layers;
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
        subresource_range
    };

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
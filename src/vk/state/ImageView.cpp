#include <vk/state/ImageView.h>

namespace aer::gfx::vk
{

ImageView::ImageView( ref_ptr<Image> in_image )
: image( in_image )
{
    if( image )
    {
        viewType                        = VK_IMAGE_VIEW_TYPE_2D; // TODO - handle other image types
        format                          = image->format;
        subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT; //TODO - get correct aspect flags
        subresourceRange.baseMipLevel   = 0;
        subresourceRange.levelCount     = image->mipLevels;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount     = image->arrayLayers;
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
    
    if( image ) image->Compile( device );

    _device = device;
    auto result = vkCreateImageView( *_device, &create_info, VK_ALLOCATOR, &_imageView );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create vkImageView: %s", ResultMessage( result ) );
}

ImageView::~ImageView()
{
    if( _imageView )
    {
        vkDestroyImageView( *_device, _imageView, VK_ALLOCATOR );
        _imageView = VK_NULL_HANDLE;
        _device = {};
    }
}

} // namespace aer::gfx::vk
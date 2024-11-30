#include <vk/state/ImageView.h>

namespace aer::vk
{

ImageView::ImageView( ref_ptr<Image> in_image, VkImageAspectFlags aspectFlags )
: image( in_image )
{
    if( image.valid() )
    {
        const auto aspect = [&]() -> VkImageAspectFlags
        {
            switch( image->format )
            {
                case VK_FORMAT_D16_UNORM_S8_UINT:
                case VK_FORMAT_D24_UNORM_S8_UINT:
                case VK_FORMAT_D32_SFLOAT_S8_UINT:
                    return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
                case VK_FORMAT_D16_UNORM:
                case VK_FORMAT_D32_SFLOAT:
                case VK_FORMAT_X8_D24_UNORM_PACK32:
                    return VK_IMAGE_ASPECT_DEPTH_BIT;
                default:
                    return VK_IMAGE_ASPECT_COLOR_BIT;
            };
        }();

        viewType                        = VK_IMAGE_VIEW_TYPE_2D; // TODO - handle other image types
        format                          = image->format;
        subresourceRange.baseMipLevel   = 0;
        subresourceRange.aspectMask     = aspectFlags ? aspectFlags : aspect;
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
        image.valid() ? *image : VK_NULL_HANDLE,
        VK_IMAGE_VIEW_TYPE_2D,
        format,
        components,
        subresourceRange
    };
    
    if( image.valid() ) image->Compile( device );
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

} // namespace aer::vk
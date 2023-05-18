#include "Image.h"

namespace AEON::Graphics::vk
{

Image::Image( VkImage image, Device* device )
: _image( image ), _device( device )
{

}

void Image::Compile( Device* device )
{
    VkImageCreateInfo create_info
    {
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        VK_NULL_HANDLE,
        flags,
        image_type,
        format,
        extent,
        mip_levels,
        arr_layers,
        samples,
        tiling,
        usage,
        share_mode,
        static_cast<uint32_t>( queue_indices.size() ),
        queue_indices.data(),
        layout
    };

    _device = device;
    auto result = vkCreateImage( *_device, &create_info, VK_ALLOCATOR, &_image );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create vkImage" );
}

Image::~Image()
{
    if( _image )
    {
        vkDestroyImage( *_device, _image, VK_ALLOCATOR );
        _image = VK_NULL_HANDLE;
    }
}


} // namespace AEON::Graphics::vk 
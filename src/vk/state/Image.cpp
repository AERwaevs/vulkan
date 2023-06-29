#include <vk/state/Image.h>

namespace aer::gfx::vk
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
        imageType,
        format,
        extent,
        mipLevels,
        arrayLayers,
        samples,
        tiling,
        usage,
        shareMode,
        static_cast<uint32_t>( queueFamilyIndices.size() ),
        queueFamilyIndices.data(),
        layout
    };
    
    _device = device;
    auto result = vkCreateImage( *_device, &create_info, VK_ALLOCATOR, &_image );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create vkImage: %s", ResultMessage( result ) );
}

Image::~Image()
{
    if( _image )
    {
        vkDestroyImage( *_device, _image, VK_ALLOCATOR );
        _image = VK_NULL_HANDLE;
    }
}


} // namespace aer::gfx::vk 
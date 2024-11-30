#include <vk/state/Image.h>

namespace aer::vk
{

Image::Image( VkImage image, ref_ptr<Device> device )
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
        sharingMode,
        static_cast<uint32_t>( queueFamilyIndices.size() ),
        queueFamilyIndices.data(),
        layout
    };
    auto result = vkCreateImage( *_device, &create_info, VK_ALLOCATOR, &_image );
    AE_FATAL_IF( result != VK_SUCCESS, "Failed to create vkImage: %s", ResultMessage( result ) );
}

Image::~Image() noexcept
{
    if( _image )
    {
        vkDestroyImage( *_device, _image, VK_ALLOCATOR );
        _image = VK_NULL_HANDLE;
    }
}


} // namespace aer::vk 
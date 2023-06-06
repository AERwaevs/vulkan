#include "Image.h"

namespace aer::Graphics::vk
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
#ifdef AEON_DEBUG
    AE_INFO
    (
        "VkImage\n"
        "{\n"
        "    imageType              = %s\n"
        "    format                 = %u\n"
        "    extent                 = {%u, %u, %u}\n"
        "    mipLevels              = %u\n"
        "    arrayLayers            = %u\n"
        "    samples                = %u\n"
        "    tiling                 = %u\n"
        "    usage                  = %u\n"
        "    sharingMode            = %u\n"
        "    queueFamilyIndexCount  = %u\n"
        "    pQueueFamilyIndices    = %s\n"
        "    initialLayout          = %u\n"
        "}\n",
        [&](){ switch( create_info.imageType ) {
            case VK_IMAGE_TYPE_1D: return "1D";
            case VK_IMAGE_TYPE_2D: return "2D";
            case VK_IMAGE_TYPE_3D: return "3D";
            default:               return "Unknown";
        }}(),
        create_info.format,
        create_info.extent.width, create_info.extent.height, create_info.extent.depth,
        create_info.mipLevels,
        create_info.arrayLayers,
        create_info.samples,
        create_info.tiling,
        create_info.usage,
        create_info.sharingMode,
        create_info.queueFamilyIndexCount,
        [&]()
        {
            std::stringstream queues;
            for( uint32_t i = 0; i < create_info.queueFamilyIndexCount; i++ )
            {
                queues << create_info.pQueueFamilyIndices[i] << " ";
            }
            return queues.str();
        }().c_str(),
        create_info.initialLayout
    );
#endif
}

Image::~Image()
{
    if( _image )
    {
        vkDestroyImage( *_device, _image, VK_ALLOCATOR );
        _image = VK_NULL_HANDLE;
    }
}


} // namespace aer::Graphics::vk 
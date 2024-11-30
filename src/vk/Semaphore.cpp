#include <vk/Semaphore.h>

namespace aer::vk
{

Semaphore::Semaphore( ref_ptr<Device> device, VkPipelineStageFlags flags, void* pNextCreateInfo )
:  _flags( flags ), _device( device )
{
    VkSemaphoreCreateInfo createInfo
    {
        VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, pNextCreateInfo, flags
    };

    auto result = vkCreateSemaphore( *_device, &createInfo, VK_ALLOCATOR, &_semaphore );
    AE_ERROR_IF( result != VK_SUCCESS, "Failed to create semaphore: %s", ResultMessage( result ) );
}

Semaphore::~Semaphore()
{
    if( _semaphore ) vkDestroySemaphore( *_device, _semaphore, VK_ALLOCATOR );
}

} // namespace aer::vk
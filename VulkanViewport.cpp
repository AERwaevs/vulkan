#include <VulkanViewport.h>
#include <VulkanRenderer.h>

namespace AEON::Graphics
{

Shared<VulkanViewport> VulkanViewport::create()
{
    return Shared<VulkanViewport>( new VulkanViewport() );
}

VulkanViewport::VulkanViewport()
{
    
}

VulkanViewport::~VulkanViewport()
{
}

} // namespace AEON::Graphics

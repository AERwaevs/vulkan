#pragma once

#include <glad/vulkan.h>

#include <Base/Base.h>

namespace aer::gfx::vk
{
    
class InputAssemblyState : public Object, public Interfaces< InputAssemblyState, ICreate, ITypeInfo >
{
public:
    InputAssemblyState( VkPrimitiveTopology, VkBool32 = VK_FALSE );

    VkPrimitiveTopology topology;
    VkBool32            primitiveRestartEnable;
};
    
} // namespace aer::gfx::vk

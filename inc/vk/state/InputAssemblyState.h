#pragma once

#include <glad/vulkan.h>

#include <Base/Base.h>

#include "GraphicsPipelineState.h"

namespace aer::gfx::vk
{
    
class InputAssemblyState : public GraphicsPipelineState, public Interfaces< InputAssemblyState, ICreate, ITypeInfo >
{
public:
    using Topology = VkPrimitiveTopology;
    Topology topology;
    VkBool32 primitiveRestartEnable;
public:
                InputAssemblyState();
    explicit    InputAssemblyState( Topology, VkBool32 = VK_FALSE );
    void        apply( VkGraphicsPipelineCreateInfo& ) const override;
private:
    // TODO move to scratch memory allocation
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState;
};
    
} // namespace aer::gfx::vk

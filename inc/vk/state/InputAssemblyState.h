#pragma once

#include "GraphicsPipelineState.h"

namespace aer::vk
{
    
struct InputAssemblyState : public GraphicsPipelineState
{
    using Topology = VkPrimitiveTopology;

                InputAssemblyState();
    explicit    InputAssemblyState( Topology, VkBool32 = VK_FALSE );

    Topology    topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    VkBool32    primitiveRestartEnable = VK_FALSE;
    
    void        apply( VkGraphicsPipelineCreateInfo& ) const override;
private:
    // TODO move to scratch memory allocation
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState;
};
    
} // namespace aer::vk

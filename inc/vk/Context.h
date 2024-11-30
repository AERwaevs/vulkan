#pragma once

#include <vk/vk.h>
#include <vk/LogicalDevice.h>
#include <vk/RenderPass.h>
#include <vk/state/GraphicsPipelineState.h>

#include <Base/memory/scratch_memory.h>

namespace aer::vk
{
    
class Context : public Object
{
public:
    explicit Context( ref_ptr<Device> in_device );
    virtual  ~Context() = default;
public:
    ref_ptr<Device>         device;
    ref_ptr<scratch_memory> scratchMemory;
    ref_ptr<RenderPass>     renderPass;

    GraphicsPipelineStates  states;
};

} // namespace aer::

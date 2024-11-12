#pragma once

#include "vk.h"
#include "Context.h"

#include "vk/PipelineLayout.h"
#include "vk/RenderPass.h"

#include "vk/state/ShaderStage.h"
#include "vk/state/GraphicsPipelineState.h"

namespace aer::gfx::vk
{

class GraphicsPipeline : public Object
{
    using Subpass = uint32_t;
public:
            GraphicsPipeline( PipelineLayout*, const ShaderStages&, const GraphicsPipelineStates&, Subpass = 0 );
    virtual ~GraphicsPipeline();
public:
    operator VkPipeline() const { return _pipeline; }
public:
    void    Compile( Context& );

public:
    ShaderStages            shaderStages;
    GraphicsPipelineStates  pipelineStates;
    ref_ptr<PipelineLayout> pipelineLayout;
    ref_ptr<RenderPass>     renderPass;
    Subpass                 subpass;
private:
    ref_ptr<Device>         _device;
    VkPipeline              _pipeline;
};

} // namespace aer::gfx::vk

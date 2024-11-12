#pragma once

#include "vk.h"
#include "LogicalDevice.h"
#include "Context.h"

namespace aer::vk
{

using SetLayouts            = std::vector<VkDescriptorSetLayout>;
using PushConstantRanges    = std::vector<VkPushConstantRange>;

class PipelineLayout : public Object
{
public:
    PipelineLayout( Device* device, const SetLayouts& in_setLayouts = {}, const PushConstantRanges& in_pushConstantRanges = {} );
    virtual ~PipelineLayout();
    
    operator VkPipelineLayout() const { return _pipelineLayout; }

    SetLayouts          setLayouts;
    PushConstantRanges  pushConstantRanges;

    void Compile( Context& context );
private:
    ref_ptr<Device>     _device;
    VkPipelineLayout    _pipelineLayout;
};

} // namespace aer::vk
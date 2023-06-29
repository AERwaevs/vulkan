#pragma once

#include <Base/Base.h>

#include <glad/vulkan.h>

#include "LogicalDevice.h"

namespace aer::gfx::vk
{

using SetLayouts            = std::vector<VkDescriptorSetLayout>;
using PushConstantRanges    = std::vector<VkPushConstantRange>;

class PipelineLayout : public Object, public Interfaces< PipelineLayout, ICreate >
{
public:
    PipelineLayout( Device* device, const SetLayouts& in_setLayouts = {}, const PushConstantRanges& in_pushConstantRanges = {} );
    
    operator VkPipelineLayout() const { return _pipelineLayout; }

    SetLayouts          setLayouts;
    PushConstantRanges  pushConstantRanges;

    void Compile( Device* device );
protected:
    ~PipelineLayout();
private:
    ref_ptr<Device>     _device;
    VkPipelineLayout    _pipelineLayout;
};

} // namespace aer::gfx::vk
#pragma once

#include <Base/Base.h>

#include <glad/vulkan.h>

#include "LogicalDevice.h"

namespace aer::gfx::vk
{

class PipelineLayout : public Object, public Interfaces< PipelineLayout, ICreate >
{
public:
    PipelineLayout( Device* device );
    
    operator VkPipelineLayout() const { return _pipelineLayout; }

protected:
    ~PipelineLayout();
private:
    ref_ptr<Device>     _device;
    VkPipelineLayout    _pipelineLayout;
};

} // namespace aer::gfx::vk
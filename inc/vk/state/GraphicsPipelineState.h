#pragma once

#include <Base/Base.h>

#include <glad/vulkan.h>

namespace aer::gfx::vk
{
    
class GraphicsPipelineState : public Object
{
public:
    virtual void    apply( VkGraphicsPipelineCreateInfo& ) const = 0;
protected:
    virtual         ~GraphicsPipelineState()                     = default;
};

} // namespace aer::gfx::vk

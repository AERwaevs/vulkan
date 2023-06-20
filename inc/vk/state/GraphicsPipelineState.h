#pragma once

#include <Base/Base.h>

#include <glad/vulkan.h>

namespace aer::gfx::vk
{
    
class GraphicsPipelineState : public Object
{
public:
    // TODO const-ify the apply function
    // ! REQUIRES scratch memory  
    virtual void    apply( VkGraphicsPipelineCreateInfo& ) {};
    virtual void    apply( VkGraphicsPipelineCreateInfo& ) const {};
protected:
    virtual         ~GraphicsPipelineState()                     = default;
};

} // namespace aer::gfx::vk

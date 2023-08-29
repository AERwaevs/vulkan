#pragma once

#include "../vk.h"

#include "ShaderModule.h"

namespace aer::gfx::vk
{

class ShaderStage : public virtual Object, public Interfaces< ShaderStage, ICreate >
{
public:
    ShaderStage
    (
        VkShaderStageFlagBits   in_stage,
        ref_ptr<ShaderModule>   in_module,
        const std::string&      in_name
    ) : stage( in_stage ), module( in_module ), name( in_name ){};

    void compile();

    VkShaderStageFlagBits stage;
    ref_ptr<ShaderModule> module;
    std::string           name;
};
    
} // namespace aer::gfx::vk
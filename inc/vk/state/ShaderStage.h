#pragma once

#include "../vk.h"

#include "ShaderModule.h"

namespace aer::vk
{

class ShaderStage : public Object
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
using ShaderStages = std::vector<ref_ptr<ShaderStage>>;

} // namespace aer::vk
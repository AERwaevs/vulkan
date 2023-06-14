#pragma once

#include <Base/Base.h>

#include <glad/vulkan.h>

#include "ShaderStage.h"

namespace aer::gfx::vk
{
    
class DynamicState : public Object, public Interfaces< DynamicState, ICreate, ITypeInfo >
{
public:
    DynamicState();
};
    
} // namespace aer::gfx::vk

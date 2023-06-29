#pragma once

#include <Base/Base.h>

#include <glad/vulkan.h>

namespace aer::gfx::vk
{

class GraphicsPipeline : public Object, Interfaces< GraphicsPipeline, ICreate >
{

};

} // namespace aer::gfx::vk

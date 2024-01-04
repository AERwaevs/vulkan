#pragma once

#include "vk.h"

namespace aer::gfx::vk
{

class GraphicsPipeline : public Object, public Interfaces< GraphicsPipeline, ICreate >
{
public:
    GraphicsPipeline();
    ~GraphicsPipeline();

};

} // namespace aer::gfx::vk

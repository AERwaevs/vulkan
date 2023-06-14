#include <vk/state/ShaderStage.h>

namespace aer::gfx::vk
{

void ShaderStage::compile()
{
    if( module ) module->compile();
}

} // namespace aer::gfx::vk
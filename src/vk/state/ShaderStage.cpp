#include <vk/state/ShaderStage.h>

namespace aer::vk
{

void ShaderStage::compile()
{
    if( module ) module->compile();
}

} // namespace aer::vk
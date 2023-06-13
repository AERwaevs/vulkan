#include "ShaderStage.h"

namespace aer::Graphics::vk
{

void ShaderStage::compile()
{
    if( module ) module->compile();
}

} // namespace aer::Graphics::vk
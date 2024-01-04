#include <vk/Context.h>

namespace aer::gfx::vk
{
    Context::Context( Device* in_device )
    : device( in_device ), scratchMemory( scratch_memory::create( 4096 ) )
    {}

} // namespace aer::gfx::vk
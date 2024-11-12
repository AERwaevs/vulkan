#include <vk/Context.h>

namespace aer::vk
{
    Context::Context( Device* in_device )
    : device( in_device ), scratchMemory( scratch_memory::create( 4096 ) )
    {}

} // namespace aer::vk
#include <vk/Context.h>

namespace aer::vk
{
    Context::Context( Device* in_device )
    : device( in_device ), scratchMemory( create<scratch_memory>(4096) )
    {}

} // namespace aer::vk
#pragma once

#include "vk.h"
#include "Instance.h"

namespace aer::vk
{

struct Surface : public Object
{
    template< typename... Args > Surface( ref_ptr<Instance>, Args... );
    ~Surface() noexcept;

    operator VkSurfaceKHR() const { return _surface; }
    auto     vk()           const { return _surface; }
    VkSurfaceKHR        _surface;
    ref_ptr<Instance>   _instance;
};

}
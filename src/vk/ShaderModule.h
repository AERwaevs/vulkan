#pragma once

#include <Base/Base.h>

#include "LogicalDevice.h"

namespace aer::Graphics::vk
{

using ByteCode = std::vector<char>;

class ShaderModule : public virtual Object, public Interfaces< ShaderModule, ICreate >
{
public:
    ShaderModule( ref_ptr<Device> device, const std::vector<char>& byte_code );
public:
    static ByteCode ReadFromFile( const std::string& path );
protected:
    virtual ~ShaderModule();

protected:
    ref_ptr<Device> _device;
    VkShaderModule  _module;
};

}
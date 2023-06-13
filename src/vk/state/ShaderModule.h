#pragma once

#include <Base/Base.h>

#include "../LogicalDevice.h"

namespace aer::Graphics::vk
{
    
struct ByteCode : public Object, public Interfaces< ByteCode, ICreate, IRead, ITypeInfo >, public std::vector<char>
{
    ByteCode( std::vector<char> bytes ) : std::vector<char>( bytes ) {};
};

class ShaderModule : public virtual Object, public Interfaces< ShaderModule, ICreate >
{
public:
    ShaderModule( ref_ptr<Device> device, const ByteCode& byte_code );
    operator VkShaderModule() const { return _module; };
public:
    void compile();
protected:
    virtual ~ShaderModule();

protected:
    ref_ptr<Device> _device;
    VkShaderModule  _module;
};

}
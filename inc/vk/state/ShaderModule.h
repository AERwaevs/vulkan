#pragma once

#include "../vk.h"
#include "../LogicalDevice.h"

namespace aer::vk
{
    
struct ByteCode : public Object, public std::vector<char>
{
    ByteCode( std::vector<char> bytes ) : std::vector<char>( bytes ) {};
};

class ShaderModule : public Object
{
public:
    ShaderModule( ref_ptr<Device> device, const ByteCode& byte_code );
    operator VkShaderModule() const { return _module; };
    virtual ~ShaderModule();
public:
    void compile(){};

protected:
    ref_ptr<Device> _device;
    VkShaderModule  _module;
};

}
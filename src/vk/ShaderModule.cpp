#include "ShaderModule.h"
#include "LogicalDevice.h"

#include <glad/vulkan.h>

namespace aer::Graphics::vk
{
    ShaderModule::ShaderModule( ref_ptr<Device> device, const std::vector<char>& byte_code )
    :   _device( device )
    {
        VkShaderModuleCreateInfo createInfo
        {
            VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            VK_NULL_HANDLE,     // pNext
            VkShaderModuleCreateFlags{ 0 },
            static_cast<uint32_t>( byte_code.size() ),
            reinterpret_cast<const uint32_t*>( byte_code.empty() ? VK_NULL_HANDLE : byte_code.data() )
        };

        auto result = vkCreateShaderModule( *device, &createInfo, VK_NULL_HANDLE, &_module );
        AE_FATAL_IF( result != VK_SUCCESS, "Failed to create shader module" );
    }

    ShaderModule::~ShaderModule()
    {
        vkDestroyShaderModule( *_device, _module, VK_NULL_HANDLE );
    }

    ByteCode ShaderModule::ReadFromFile( const std::string& path )
    {
        std::ifstream file( path, std::ios::ate | std::ios::binary );
        AE_WARN_IF( !file.is_open(), "Failed to open %s", path.c_str() );

        std::size_t         file_size = static_cast<std::size_t>( file.tellg() );
        std::vector<char>   code( file_size );

        file.seekg(0);
        file.read( code.data(), file_size );
        file.close();

        return code;
    }
    
} // namespace aer::Graphics


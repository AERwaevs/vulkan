#include <Graphics/VulkanViewport.h>
#include <Graphics/VulkanRenderer.h>

#include <vk/PipelineLayout.h>

#include <vk/state/ShaderModule.h>
#include <vk/state/ShaderStage.h>
#include <vk/state/DynamicState.h>
#include <vk/state/VertexInputState.h>
#include <vk/state/InputAssemblyState.h>
#include <vk/state/ViewportState.h>
#include <vk/state/RasterizationState.h>
#include <vk/state/MultisampleState.h>
#include <vk/state/DepthStencilState.h>
#include <vk/state/ColorBlendState.h>

namespace aer::gfx
{

template<>
ref_ptr<Viewport> Viewport::create< API::Vulkan >( Window* window )
{
    return VulkanViewport::create_if( VulkanSupported(), window );
}

const inline auto GetQueueSettings( vk::PhysicalDevice* physical_device, vk::Surface* surface )
{
    auto [ present_family, graphics_family ]
    {
        physical_device->GetQueueFamilies( VK_QUEUE_GRAPHICS_BIT, surface )
    };
    return vk::QueueSettings
    {
        vk::QueueSetting{ graphics_family,{ 1.0f } },
        vk::QueueSetting{ present_family, { 1.0f } },
    };
}

VulkanViewport::VulkanViewport( Window* window )
:   Viewport( window, VulkanRenderer::get_or_create() ),
    _instance( vk::Instance::get_or_create() ),
    _surface( vk::Surface::create( _instance, window->native<vk::Window_t>() ) ),
    _physical_device
    (
        _instance->physical_device( VK_QUEUE_GRAPHICS_BIT, _surface, _instance->DevicePreferences )
    ),
    _device( vk::Device::create
    (
        _physical_device, _surface, GetQueueSettings( _physical_device, _surface )
    ) )
{
    //? if _swapchain is created during initialisation, it seems to execute before _swapchain_prefs is constructed
    _swapchain = vk::Swapchain::create
    (
        _physical_device, _device, _surface,
        window->width(), window->height(), _swapchain_prefs
    );

    // get shader bytecode
    auto vert_code = vk::ByteCode::read( "shaders/tri.vert.spv" );
    auto frag_code = vk::ByteCode::read( "shaders/tri.frag.spv" );

    // create shader modules
    auto vert_module = vk::ShaderModule::create( _device, *vert_code );
    auto frag_module = vk::ShaderModule::create( _device, *frag_code );

    // create shader stages
    ref_ptr<vk::ShaderStage> stages[]
    {
        vk::ShaderStage::create( VK_SHADER_STAGE_VERTEX_BIT, vert_module, "main" ),
        vk::ShaderStage::create( VK_SHADER_STAGE_FRAGMENT_BIT, frag_module, "main" )
    };

    auto dynamic_state          = vk::DynamicState::create();
    auto vertex_input_state     = vk::VertexInputState::create();
    auto input_assembly_state   = vk::InputAssemblyState::create();
    auto viewport_state         = vk::ViewportState::create( _swapchain->extent() );
    auto rasterization_state    = vk::RasterizationState::create();
    auto multisample_state      = vk::MultisampleState::create( VK_SAMPLE_COUNT_4_BIT );
    auto depthstencil_state     = vk::DepthStencilState::create();
    auto colorblend_state       = vk::ColorBlendState::create();
    auto pipeline_layout        = vk::PipelineLayout::create( _device );
         pipeline_layout->Compile( _device );
}

VulkanViewport::~VulkanViewport()
{
}

} // namespace aer::gfx

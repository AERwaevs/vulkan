#include <Graphics/VulkanViewport.h>
#include <Graphics/VulkanRenderer.h>

#include <vk/RenderPass.h>
#include <vk/PipelineLayout.h>
#include <vk/GraphicsPipeline.h>

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
    return VulkanViewport::create_if( vk_supported(), window );
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
    _physical_device( _instance->physical_device( VK_QUEUE_GRAPHICS_BIT, _surface ) ),
    _device( vk::Device::create
    (
        _physical_device, _surface, GetQueueSettings( _physical_device, _surface )
    ) ),
    _context( vk::Context::create( _device ) )
{
    using namespace vk;

    //? if _swapchain is created during initialisation, it seems to execute before _swapchain_prefs is constructed
    _swapchain = Swapchain::create
    (
        _physical_device, _device, _surface,
        window->width(), window->height(), _swapchain_prefs
    );

    _context->renderPass = RenderPass::create( _device, _swapchain->format(), VK_FORMAT_D32_SFLOAT );
    _context->states =
    {
        DynamicState::create(),
        VertexInputState::create(),
        InputAssemblyState::create(),
        ViewportState::create( _swapchain->extent() ),
        RasterizationState::create(),
        MultisampleState::create( VK_SAMPLE_COUNT_4_BIT ),  //TODO - this needs to match the renderpass color and/or depth attachment
        DepthStencilState::create(),
        ColorBlendState::create()
    };

    //* create shaders
    auto vert_code   = ByteCode::read( "shaders/tri.vert.spv" );
    auto frag_code   = ByteCode::read( "shaders/tri.frag.spv" );
    auto vert_module = ShaderModule::create( _device, *vert_code );
    auto frag_module = ShaderModule::create( _device, *frag_code );
    ShaderStages stages
    {
        ShaderStage::create( VK_SHADER_STAGE_VERTEX_BIT, vert_module, "main" ),
        ShaderStage::create( VK_SHADER_STAGE_FRAGMENT_BIT, frag_module, "main" )
    };
    
    _pipelineLayout = PipelineLayout::create( _context->device );
    _pipelineLayout->Compile( *_context );
    _graphicsPipeline = GraphicsPipeline::create( _pipelineLayout, stages, _context->states );
    _graphicsPipeline->Compile( *_context );
}

VulkanViewport::~VulkanViewport()
{
}

} // namespace aer::gfx

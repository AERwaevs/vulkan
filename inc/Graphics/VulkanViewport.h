#pragma once

#include <Graphics/Viewport.h>
#include <Graphics/Window.h>

#include <vk/Instance.h>
#include <vk/Surface.h>
#include <vk/LogicalDevice.h>
#include <vk/Swapchain.h>
#include <vk/Context.h>
#include "vk/PipelineLayout.h"
#include "vk/GraphicsPipeline.h"

#include "VulkanRenderer.h"

namespace aer::gfx
{
    class VulkanViewport : public Viewport, public Interfaces< VulkanViewport, ICreateIf >
    {
        friend class VulkanRenderer;

    public:
        VulkanViewport( Window* window );
        ~VulkanViewport();

        VulkanRenderer& Renderer() const { return static_cast<VulkanRenderer&>(*_renderer); }
    private:
        ref_ptr<vk::Instance>           _instance;
        ref_ptr<vk::Surface>            _surface;
        ref_ptr<vk::PhysicalDevice>     _physical_device;
        ref_ptr<vk::Device>             _device;
        ref_ptr<vk::Swapchain>          _swapchain;
        ref_ptr<vk::Context>            _context;
        ref_ptr<vk::PipelineLayout>     _pipelineLayout;
        ref_ptr<vk::GraphicsPipeline>   _graphicsPipeline;
        
        vk::SwapchainPreferences    _swapchain_prefs{};
    };
} // namespace aer::gfx

#pragma once

#include <vk/vk.h>
#include <vk/RenderPass.h>

#include <vk/state/ImageView.h>

namespace aer::gfx::vk
{
    
class Framebuffer : public Object
{
public:
    Framebuffer( ref_ptr<RenderPass> renderPass, const ImageViews& attachments, uint32_t width, uint32_t height, uint32_t layers );
    virtual ~Framebuffer();

    operator VkFramebuffer() const { return _framebuffer; }

    uint32_t width() const   { return _width; }
    uint32_t height() const  { return _height; }
    uint32_t layers() const  { return _layers; }

protected:
    VkDevice        _device;
    VkFramebuffer   _framebuffer;

    const uint32_t  _width;
    const uint32_t  _height;
    const uint32_t  _layers;
};
using Framebuffers = std::vector<ref_ptr<Framebuffer>>;

} // namespace aer::gfx::vk
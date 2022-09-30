#pragma once

namespace Mirage
{
    struct FramebufferSpecs
    {
        uint32_t Width, Height;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };
    
    class Framebuffer
    {
    public:
        static Ref<Framebuffer> Create(const FramebufferSpecs& specs);
        
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual uint32_t GetColorAttachmentRendererID() const = 0;
        
        virtual const FramebufferSpecs& GetSpecs() const = 0; 
    };
}

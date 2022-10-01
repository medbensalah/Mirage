#pragma once
#include "Mirage/Renderer/Framebuffer.h"

namespace Mirage
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecs& specs);
        virtual ~OpenGLFramebuffer();

        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;

        inline virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
        inline virtual const FramebufferSpecs& GetSpecs() const override { return m_Specs;}
    private:
        uint32_t m_RendererID = 0;

        uint32_t m_ColorAttachment = 0;
        uint32_t m_DepthAttachment = 0;
        FramebufferSpecs m_Specs;
    };
}

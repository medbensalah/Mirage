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

        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

        virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;
        virtual void ClearAttachment(uint32_t attachmentIndex, float value) override;
        
        inline virtual uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment; }
        inline virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override
        {
            MRG_CORE_ASSERT(index <= m_ColorAttachments.size());
            return m_ColorAttachments[index];
        }
        inline virtual const FramebufferSpecs& GetSpecs() const override { return m_Specs;}
    private:
        uint32_t m_RendererID = 0;

        FramebufferSpecs m_Specs;

        std::vector<FramebufferTextureSpecs> m_ColorAttachmentSpecs;
        FramebufferTextureSpecs m_DepthAttachmentSpec = FramebufferTextureFormat::None;

        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment = 0;
    };
}

﻿#pragma once

namespace Mirage
{
    enum class FramebufferTextureFormat
    {
        None = 0,

        // Color
        RGBA8,
        RED_INTEGER,

        // Depth/Stencil
        DEPTH24STENCIL8,

        // Default
        Depth = DEPTH24STENCIL8
    };

    struct FramebufferTextureSpecs
    {
        FramebufferTextureSpecs() = default;
        FramebufferTextureSpecs(FramebufferTextureFormat format)
            : TextureFormat(format) {}

        FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
        // uint32_t Samples = 1;
        // bool SwapChainTarget = false;
    };

    struct FrameBufferAttachmentSpecs
    {
        FrameBufferAttachmentSpecs() = default;
        FrameBufferAttachmentSpecs(std::initializer_list<FramebufferTextureSpecs> attachmentSpecs)
            : AttachmentSpecs(attachmentSpecs) {}

        std::vector<FramebufferTextureSpecs> AttachmentSpecs;
    };
    
    struct FramebufferSpecs
    {
        uint32_t Width = 0, Height = 0;
        FrameBufferAttachmentSpecs Attachments;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };
    
    class Framebuffer
    {
    public:
        static Ref<Framebuffer> Create(const FramebufferSpecs& specs);
        virtual ~Framebuffer() = default;
        
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

        virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;
        virtual void ClearAttachment(uint32_t attachmentIndex, float value) = 0;
        
        virtual uint32_t GetDepthAttachmentRendererID() const = 0;
        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
        
        virtual const FramebufferSpecs& GetSpecs() const = 0; 
    };
}

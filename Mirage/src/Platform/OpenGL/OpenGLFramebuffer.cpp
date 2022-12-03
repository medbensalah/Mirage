#include "MrgPch.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Mirage
{
    static const uint32_t s_MaxFramebufferSize = 8192;

    namespace Utils
    {
        static GLenum TextureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }
        
        static void CreateTextures(bool multisample, uint32_t* outID, uint32_t count)
        {
                glCreateTextures(TextureTarget(multisample), count, outID);
        }

        static void BindTexture(bool multisample, uint32_t id)
        {
            glBindTexture(TextureTarget(multisample), id);
        }

        static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
        {
            bool multiSample = samples > 1;
            if (multiSample)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multiSample), id, 0);
        }

        static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
        {
            bool multiSample = samples > 1;
            if (multiSample)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multiSample), id, 0);
        }

        static bool IsDepthFormat(FramebufferTextureFormat format)
        {
            switch (format)
            {
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                return true;
            default:
                return false;
            }
        }

        static GLenum MrgFbTextureFormatToGL(FramebufferTextureFormat format)
        {
            switch (format)
            {
            case FramebufferTextureFormat::RGBA8:               return GL_RGBA8;
            case FramebufferTextureFormat::RED_INTEGER:         return GL_RED_INTEGER;
            case FramebufferTextureFormat::DEPTH24STENCIL8:     return GL_DEPTH24_STENCIL8;
            }

            MRG_CORE_ASSERT(false);
            return 0;
        }
    }
    
    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecs& specs)
        : m_Specs(specs)
    {
        for (auto format : m_Specs.Attachments.AttachmentSpecs)
        {
            if(!Utils::IsDepthFormat(format.TextureFormat))
                m_ColorAttachmentSpecs.emplace_back(format);
            else
                m_DepthAttachmentSpec = format;
        }
        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFramebuffer::Invalidate()
    {
        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);

            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
        }
        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        // Attachments
        bool multiSample = m_Specs.Samples > 1;
        if (m_ColorAttachmentSpecs.size())
        {
            m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());

            Utils::CreateTextures(multiSample, m_ColorAttachments.data(), m_ColorAttachments.size());
            
            for (size_t i = 0; i < m_ColorAttachments.size(); ++i)
            {
                Utils::BindTexture(multiSample, m_ColorAttachments[i]);
                switch (m_ColorAttachmentSpecs[i].TextureFormat)
                {
                case FramebufferTextureFormat::RGBA8:
                    Utils::AttachColorTexture(m_ColorAttachments[i], m_Specs.Samples, GL_RGBA8, GL_RGBA, m_Specs.Width, m_Specs.Height, i); 
                    break;
                case FramebufferTextureFormat::RED_INTEGER:
                    Utils::AttachColorTexture(m_ColorAttachments[i], m_Specs.Samples, GL_R32I, GL_RED_INTEGER, m_Specs.Width, m_Specs.Height, i);
                    break;
                }
            }            
        }

        if (m_DepthAttachmentSpec.TextureFormat != FramebufferTextureFormat::None)
        {
            Utils::CreateTextures(multiSample, &m_DepthAttachment, 1);
            Utils::BindTexture(multiSample, m_DepthAttachment);
            switch (m_DepthAttachmentSpec.TextureFormat)
            {
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                Utils::AttachDepthTexture(m_DepthAttachment, m_Specs.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specs.Width, m_Specs.Height);
                break;
            }
        }

        if (m_ColorAttachments.size() > 1)
        {
            MRG_CORE_ASSERT(m_ColorAttachments.size() <= 4);
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(m_ColorAttachments.size(), buffers);
        }
        else if(m_ColorAttachments.empty())
        {
            // only depth-pass
            glDrawBuffer(GL_NONE);
        }

        MRG_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                        "Framebuffer isn't complete");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specs.Width, m_Specs.Height);
    }

    void OpenGLFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width <= 0 || height <= 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
        {
            MRG_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
            return;
        }

        m_Specs.Width = width;
        m_Specs.Height = height;

        Invalidate();
    }

    int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
    {
        MRG_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());
        
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
    {
        MRG_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());

        auto& spec = m_ColorAttachmentSpecs[attachmentIndex];
        glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::MrgFbTextureFormatToGL(spec.TextureFormat),
                        GL_INT, &value);
    }

    void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, float value)
    {
        MRG_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());

        auto& spec = m_ColorAttachmentSpecs[attachmentIndex];
        glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::MrgFbTextureFormatToGL(spec.TextureFormat),
                        GL_FLOAT, &value);
    }
}

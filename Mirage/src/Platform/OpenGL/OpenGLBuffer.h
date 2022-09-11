#pragma once

#include "Mirage/Renderer/Buffer.h"

namespace Mirage
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                          ///
    ///                                       Vertex Buffer                                      ///
    ///                                                                                          ///
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    MIRAGE_CLASS OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const;
        virtual void Unbind() const;

    private:
        uint32_t m_RendererID;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                          ///
    ///                                       Index Buffer                                       ///
    ///                                                                                          ///
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    MIRAGE_CLASS OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const;
        virtual void Unbind() const;

        inline virtual uint32_t GetCount() const override { return m_Count; }
        
    private:
        uint32_t m_Count;
        uint32_t m_RendererID;
    };
}

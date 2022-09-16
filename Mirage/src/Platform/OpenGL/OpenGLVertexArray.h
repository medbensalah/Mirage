#pragma once

#include "Mirage/Renderer/VertexArray.h"

namespace Mirage
{
    MIRAGE_CLASS OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& VertexBuffer) override;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& IndexBuffer) override;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
        
    private:
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;

        unsigned int m_RendererID;
    };
}

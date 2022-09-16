#pragma once

#include "Mirage/Renderer/Buffer.h"

namespace Mirage
{
    MIRAGE_CLASS VertexArray
    {
        public:
        virtual ~VertexArray()
        {
        }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& VertexBuffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& IndexBuffer) = 0;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

        static VertexArray* Create();
    };
}

#include "MrgPch.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Mirage
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Bool:          return GL_BOOL;
        case ShaderDataType::Int1:          return GL_INT;
        case ShaderDataType::Int2:          return GL_INT;
        case ShaderDataType::Int3:          return GL_INT;
        case ShaderDataType::Int4:          return GL_INT;
        case ShaderDataType::Float1:        return GL_FLOAT;
        case ShaderDataType::Float2:        return GL_FLOAT;
        case ShaderDataType::Float3:        return GL_FLOAT;
        case ShaderDataType::Float4:        return GL_FLOAT;
        case ShaderDataType::Mat3_s:        return GL_FLOAT;
        case ShaderDataType::Mat4_s:        return GL_FLOAT;
        }

        MRG_CORE_ASSERT(false, "Unknown Shader Data Type!");
        return 0;
    }
    
    OpenGLVertexArray::OpenGLVertexArray()
    {
        MRG_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        MRG_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &m_RendererID);
    }
    
    void OpenGLVertexArray::Bind() const
    {
        MRG_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        MRG_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& VertexBuffer)
    {
        MRG_PROFILE_FUNCTION();

        MRG_CORE_ASSERT(VertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        glBindVertexArray(m_RendererID);
        VertexBuffer->Bind();

        const auto& layout = VertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            switch (element.Type)
            {
            case ShaderDataType::Float1:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
                {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribPointer(m_VertexBufferIndex,
                                          element.GetComponentCount(),
                                          ShaderDataTypeToOpenGLBaseType(element.Type),
                                          element.Normalized ? GL_TRUE : GL_FALSE,
                                          layout.GetStride(),
                                          (const void*)element.Offset);
                    m_VertexBufferIndex++;
                    break;
                }
            case ShaderDataType::Int1:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribIPointer(m_VertexBufferIndex,
                                          element.GetComponentCount(),
                                          ShaderDataTypeToOpenGLBaseType(element.Type),
                                          layout.GetStride(),
                                          (const void*)element.Offset);
                    m_VertexBufferIndex++;
                    break;
                }
            case ShaderDataType::Mat3_s:
            case ShaderDataType::Mat4_s:
                {
                    uint8_t count = element.GetComponentCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(m_VertexBufferIndex);
                        glVertexAttribPointer(m_VertexBufferIndex,
                                              count,
                                              ShaderDataTypeToOpenGLBaseType(element.Type),
                                              element.Normalized ? GL_TRUE : GL_FALSE,
                                              layout.GetStride(),
                                              (const void*)(element.Offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(m_VertexBufferIndex, 1);
                        m_VertexBufferIndex++;
                    }
                    break;
                }
            default:
                MRG_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
        }

        m_VertexBuffers.push_back(VertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& IndexBuffer)
    {
        MRG_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        IndexBuffer->Bind();

        m_IndexBuffer = IndexBuffer;
    }
}

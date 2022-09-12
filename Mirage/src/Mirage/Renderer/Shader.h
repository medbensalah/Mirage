#pragma once
#include "Mirage/Math.h"

namespace Mirage
{
    MIRAGE_CLASS Shader
    {
    public:
        Shader(const std::string& vertexSource, const std::string& fragmentSource);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void UploadUniformMat4(const std::string& name, const Mat4& matrix);
        
    private:
        uint32_t m_RendererID;
    };
}

#pragma once

#include "Mirage/Renderer/Shader.h"
#include "Mirage/Math.h"

namespace Mirage
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        void UploadUniformInt1(const std::string& name, const int& vector);
        
        void UploadUniformFloat1(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const Vec2& vector);
        void UploadUniformFloat3(const std::string& name, const Vec3& vector);
        void UploadUniformFloat4(const std::string& name, const Vec4& vector);
        
        void UploadUniformMat3(const std::string& name, const Mat3& matrix);
        void UploadUniformMat4(const std::string& name, const Mat4& matrix);

    private:
        uint32_t m_RendererID;
    };
}

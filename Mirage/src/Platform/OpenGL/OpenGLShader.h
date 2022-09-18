#pragma once

#include "Mirage/Renderer/Shader.h"
#include "Mirage/Core/Math.h"

//TODO: Remove
typedef unsigned int GLenum;

namespace Mirage
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        inline virtual const std::string& GetName() const override { return m_Name;}
        
        void UploadUniformInt(const std::string& name, const int& vector);
        
        void UploadUniformFloat1(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const Vec2& vector);
        void UploadUniformFloat3(const std::string& name, const Vec3& vector);
        void UploadUniformFloat4(const std::string& name, const Vec4& vector);
        
        void UploadUniformMat3(const std::string& name, const Mat3& matrix);
        void UploadUniformMat4(const std::string& name, const Mat4& matrix);

    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
        
        std::string m_Name;
        uint32_t m_RendererID;
    };
}

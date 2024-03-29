﻿#pragma once

#include "Mirage/Renderer/Shader.h"
#include "Mirage/Math/glmTypes.h"

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

        virtual void SetInt(const std::string& name, int vector) override;
        virtual void SetInt2(const std::string& name, const VecI2& vector) override;
        virtual void SetInt3(const std::string& name, const VecI3& vector) override;
        virtual void SetInt4(const std::string& name, const VecI4& vector) override;
        
        virtual void SetIntArray(const std::string& name, int* arr, uint32_t count) override;
        
        virtual void SetFloat(const std::string& name, float vector) override;
        virtual void SetFloat2(const std::string& name, const Vec2& vector) override;
        virtual void SetFloat3(const std::string& name, const Vec3& vector) override;
        virtual void SetFloat4(const std::string& name, const Vec4& vector) override;
        
        virtual void SetMat3(const std::string& name, const Mat3& matrix) override;
        virtual void SetMat4(const std::string& name, const Mat4& matrix) override;
        
        inline virtual const std::string& GetName() const override { return m_Name;}
        
        void UploadUniformInt(const std::string& name, int value);
        void UploadUniformInt2(const std::string& name, const VecI2& vector);
        void UploadUniformInt3(const std::string& name, const VecI3& vector);
        void UploadUniformInt4(const std::string& name, const VecI4& vector);
        
        void UploadUniformIntArray(const std::string& name, int* arr, uint32_t count);
        
        void UploadUniformFloat(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const Vec2& vector);
        void UploadUniformFloat3(const std::string& name, const Vec3& vector);
        void UploadUniformFloat4(const std::string& name, const Vec4& vector);
        
        void UploadUniformMat3(const std::string& name, const Mat3& matrix);
        void UploadUniformMat4(const std::string& name, const Mat4& matrix);

    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

        void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
        void CompileOrGetOpenGLBinaries();
        void CreateProgram();
        void Reflect(GLenum stage, const std::vector<uint32_t>& shaderData);

    private:
        uint32_t m_RendererID;
        
        std::string m_FilePath;
        std::string m_Name;
        
        std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV;
        std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;

        std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;
    };

}

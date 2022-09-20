#pragma once

#include "Mirage/Core/Math.h"

namespace Mirage
{
    MIRAGE_CLASS Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetInt(const std::string& name, int value) = 0;
        virtual void SetInt2(const std::string& name, const VecI2& vector) = 0;
        virtual void SetInt3(const std::string& name, const VecI3& value) = 0;
        virtual void SetInt4(const std::string& name, const VecI4& value) = 0;
        
        virtual void SetFloat(const std::string& name, float value) = 0;
        virtual void SetFloat2(const std::string& name, const Vec2& vector) = 0;
        virtual void SetFloat3(const std::string& name, const Vec3& vector) = 0;
        virtual void SetFloat4(const std::string& name, const Vec4& vector) = 0;
        
        virtual void SetMat3(const std::string& name, const Mat3& matrix) = 0;
        virtual void SetMat4(const std::string& name, const Mat4& matrix) = 0;
        
        virtual const std::string& GetName() const = 0;
        
        static Ref<Shader> Create(const std::string& filepath);
        static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    };

    MIRAGE_CLASS ShaderLibrary
    {
    public:
        void Add(const std::string& name, const Ref<Shader>& shader);
        void Add(const Ref<Shader>& shader);

        Ref<Shader> Load(const std::string& filepath);
        Ref<Shader> Load(const std::string& name, const std::string& filepath);

        Ref<Shader> Get(const std::string& name);

        inline bool Exists(const std::string& name) const
        {
            return m_Shaders.find(name) != m_Shaders.end();
        }
    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };
}

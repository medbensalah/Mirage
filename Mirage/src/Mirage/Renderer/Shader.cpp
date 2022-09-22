#include <MrgPch.h>

#include "Mirage/Renderer/Shader.h"
#include "Mirage/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Mirage
{
    Ref<Shader> Shader::Create(const std::string& filepath)
    {
        switch (Renderer::GetGfxApi())
        {
        case RenderAPI::API::None:
            MRG_CORE_ASSERT(false, "No graphics API selected!");
            return nullptr;

        case RenderAPI::API::OpenGL:
            return CreateRef<OpenGLShader>(filepath);
        }

        MRG_CORE_ASSERT(false, "Unknown graphics API!");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSource,
                               const std::string& fragmentSource)
    {
        switch (Renderer::GetGfxApi())
        {
        case RenderAPI::API::None:
            MRG_CORE_ASSERT(false, "No graphics API selected!");
            return nullptr;

        case RenderAPI::API::OpenGL:
            return CreateRef<OpenGLShader>(name, vertexSource, fragmentSource);
        }

        MRG_CORE_ASSERT(false, "Unknown graphics API!");
        return nullptr;
    }

    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
    {
        MRG_CORE_ASSERT(!Exists(name), "Shader already exists!");
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        auto& name = shader->GetName();
        Add(name, shader);
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        MRG_CORE_ASSERT(Exists(name), "Shader not found!");
        return m_Shaders[name];
    }
}

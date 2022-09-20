#include "MrgPch.h"

#include "OpenGLShader.h"

#include <glad/glad.h>

#include "glm/gtc/type_ptr.inl"

namespace Mirage
{
    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel")
            return GL_FRAGMENT_SHADER;

        MRG_CORE_ASSERT(false, "Unknown shader type!");
        return 0;
    }
    
    OpenGLShader::OpenGLShader(const std::string& filepath)
    {
        std::string source = ReadFile(filepath);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);
        
        auto lastSladh = filepath.find_last_of("/\\");
        lastSladh = lastSladh == std::string::npos ? 0 : lastSladh + 1;
        auto lastDot = filepath.rfind('.');
        int count = lastDot == std::string::npos ? filepath.size() - lastSladh : lastDot - lastSladh;
        m_Name = filepath.substr(lastSladh, count);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
        : m_Name(name)
    {
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSource;
        sources[GL_FRAGMENT_SHADER] = fragmentSource;
        Compile(sources);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            MRG_CORE_ERROR("Could not open file: {0}", filepath);
        }
        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
            MRG_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
            std::string type = source.substr(begin, eol - begin);
            MRG_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
            MRG_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
            pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line
            shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        GLuint program = glCreateProgram();
        MRG_CORE_ASSERT(shaderSources.size() <= 2, "Mirage supports onlu up to 2 shaders");
        std::array<GLenum, 2> glShaderIDs;
        int glShaderIDIndex = 0;
        for(auto& kv : shaderSources)
        {
            GLenum type = kv.first;
            const std::string& source = kv.second;

            GLuint shader = glCreateShader(type);
            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                MRG_CORE_ERROR("{0}", infoLog.data());
                MRG_CORE_ASSERT(false, "Shader compilation failed!");
                break;
            }

            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }
        
        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            for (auto id : glShaderIDs)
                glDeleteShader(id);

            MRG_CORE_ERROR("{0}", infoLog.data());
            MRG_CORE_ASSERT(false, "Shader link failed!");
            return;
        }
        
        for (auto id : glShaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }

        
        m_RendererID = program;
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string& name, int vector)
    {
        UploadUniformInt(name, vector);
    }

    void OpenGLShader::SetInt2(const std::string& name, const glm::ivec2& vector)
    {
        UploadUniformInt2(name, vector);
    }

    void OpenGLShader::SetInt3(const std::string& name, const glm::ivec3& vector)
    {
        UploadUniformInt3(name, vector);
    }

    void OpenGLShader::SetInt4(const std::string& name, const glm::ivec4& vector)
    {
        UploadUniformInt4(name, vector);
    }

    void OpenGLShader::SetFloat(const std::string& name, float vector)
    {
        UploadUniformFloat(name, vector);
    }

    void OpenGLShader::SetFloat2(const std::string& name, const glm::fvec2& vector)
    {
        UploadUniformFloat2(name, vector);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& vector)
    {
        UploadUniformFloat3(name, vector);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& vector)
    {
        UploadUniformFloat4(name, vector);
    }

    void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix)
    {
        UploadUniformMat3(name, matrix);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
    {
        UploadUniformMat4(name, matrix);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformInt2(const std::string& name, const glm::ivec2& vector)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2i(location, vector.x, vector.y);
    }

    void OpenGLShader::UploadUniformInt3(const std::string& name, const glm::ivec3& vector)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3i(location, vector.x, vector.y, vector.z);
    }

    void OpenGLShader::UploadUniformInt4(const std::string& name, const glm::ivec4& vector)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4i(location, vector.x, vector.y, vector.z, vector.w);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, vector.x, vector.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vector)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, vector.x, vector.y, vector.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vector)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, vector.r, vector.g, vector.b, vector.a);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

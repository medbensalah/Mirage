#pragma once

#include "Scene.h"

namespace Mirage
{
    class SceneSerializer
    {
    public:
        SceneSerializer(const Ref<Scene>& scene);
        
        void SerializeText(const std::string& filepath);
        bool DeserializeText(const std::string& filepath);
        
        void SerializeBinary(const std::string& filepath);
        bool DeserializeBinary(const std::string& filepath);

    private:
        Ref<Scene> m_Scene;
    };
}

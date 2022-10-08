#pragma once

#include <enTT.hpp>

namespace Mirage
{
    class SceneObject;
    class Scene
    {
    public:
        Scene();
        ~Scene();

        SceneObject CreateEntity(const std::string& name ="");

        void OnUpdate(float DeltaTime);

    private:
        entt::registry m_Registry;

        friend class SceneObject;
    };
}

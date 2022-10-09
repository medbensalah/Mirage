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
        void OnViewportResize(uint32_t width, uint32_t height);

    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth;
        uint32_t m_ViewportHeight;
        
        friend class SceneObject;
    };
}

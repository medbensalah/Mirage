#include "MrgPch.h"

#include "Scene.h"
#include "SceneObject.h"
#include "Components.h"
#include "Mirage/Renderer/Renderer2D.h"

namespace Mirage
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    SceneObject Scene::CreateEntity(const std::string& name)
    {
        SceneObject sceneObject {m_Registry.create(), this};
        sceneObject.AddComponent<TransformComponent>();
        auto& tag = sceneObject.AddComponent<TagComponent>(name);
        
        return sceneObject;
    }

    void Scene::OnUpdate(float DeltaTime)
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::Draw::Quad(transform, sprite.Color);
        }
    }
}

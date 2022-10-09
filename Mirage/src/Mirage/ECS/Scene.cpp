#include "MrgPch.h"

#include "Scene.h"
#include "SceneObject.h"
#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteRendererComponent.h"
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
        SceneObject sceneObject {m_Registry.create(), this, name};
        sceneObject.AddComponent<TransformComponent>();
        
        return sceneObject;
    }

    void Scene::OnUpdate(float DeltaTime)
    {

        //Render 2D
        Camera* mainCamera = nullptr;
        Mat4* cameraTransform = nullptr;
        {
            auto group = m_Registry.group(entt::get<TransformComponent, CameraComponent>);
            for(auto entity : group)
            {
                auto& [transform, cam] = group.get<TransformComponent, CameraComponent>(entity);
                
                if (cam.IsMain)
                {
                    mainCamera = &(cam.Camera);
                    cameraTransform = &(transform.Trannsform);
                    break;
                }
            }
        }
        if(mainCamera)
        {
            Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        
            for (auto entity : group)
            {
                auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::Draw::Quad(transform, sprite.Color);
                // MRG_CORE_INFO((unsigned int)entity);
            }

            Renderer2D::EndScene();
        }
    }
}

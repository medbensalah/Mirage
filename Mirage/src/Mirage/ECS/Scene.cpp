#include "MrgPch.h"

#include "Scene.h"
#include "SceneObject.h"
#include "Components/CameraComponent.h"
#include "Components/NativeScriptComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/TagComponent.h"
#include "Mirage/Renderer/Renderer2D.h"

namespace Mirage
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    SceneObject Scene::CreateSceneObject(const std::string& name)
    {
        SceneObject sceneObject {m_Registry.create(), this};
        sceneObject.AddComponent<TransformComponent>();
        auto& tag = sceneObject.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        
        return sceneObject;
    }
    
    void Scene::DestroySceneObject(SceneObject& entity)
    {
        entity.Destroy();
    }

    void Scene::OnUpdate(float DeltaTime)
    {
        //Update scripts
        {
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
            {
                //TODO: Move to Scene::OnPlay
               if(!nsc.Instance)
               {
                   nsc.Instance = nsc.InstantiateScript();
                   nsc.Instance->m_SceneObject = SceneObject{entity, this};
                   nsc.Instance->OnCreate();
               }

                nsc.Instance->OnUpdate(DeltaTime);
            });
        }

        //Render 2D
        Camera* mainCamera = nullptr;
        Mat4 cameraTransform;
        {
            auto group = m_Registry.group(entt::get<TransformComponent, CameraComponent>);
            for(auto entity : group)
            {
                auto [transform, cam] = group.get<TransformComponent, CameraComponent>(entity);
                
                if (cam.IsMain)
                {
                    mainCamera = &(cam.Camera);
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
        }


        
        if(mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, cameraTransform);
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::Draw::Quad(transform.GetTransform(), sprite.Color);
            }

            Renderer2D::EndScene();
        }
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        // Resize non fixer aspect ratio cameras
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
            {
                cameraComponent.Camera.SetViewportSize(width, height);
            }
        }
    }
}

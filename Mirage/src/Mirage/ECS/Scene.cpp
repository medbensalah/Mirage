﻿#include "MrgPch.h"

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
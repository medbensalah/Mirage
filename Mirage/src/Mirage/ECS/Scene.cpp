#include "MrgPch.h"

#include "Scene.h"
#include "SceneObject.h"
#include "Components/Rendering/CameraComponent.h"
#include "Components/Base/NativeScriptComponent.h"
#include "Components/Base/TransformComponent.h"
#include "Components/Rendering/SpriteRendererComponent.h"
#include "Components/Base/TagComponent.h"
#include "Mirage/Renderer/Renderer2D.h"

#include "box2d/b2_world.h"

namespace Mirage
{
	Scene::~Scene()
	{
		for (auto& entity : m_Hierarchy)
		{
			m_Registry.destroy(entity.first);
		}
	}

    SceneObject Scene::CreateSceneObject(const std::string& name)
    {        
        SceneObject sceneObject = {m_Registry.create(), this};
        Relationship r;
        m_Hierarchy.emplace(sceneObject, r);
        auto& transform = sceneObject.AddComponent<TransformComponent>(this);
        auto& tag = sceneObject.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        
        return sceneObject;
    }

    SceneObject Scene::CreateChildSceneObject(entt::entity parent, const std::string& name)
    {
        SceneObject so = CreateSceneObject(name);

        SceneObject p = {parent, this};
        p.AddChild(so);
        return so;
    }

    void Scene::DestroySceneObject(SceneObject& entity)
    {
        entity.Destroy();
    }

    void Scene::OnRuntimeStart()
    {
		m_PhysicsWorld = new b2World(b2Vec2(0.0f, m_Gravity));
		for (auto& [entity, component] : m_Registry.view<NativeScriptComponent>().each())
		{
			component.InstantiateScript();
		}
    }

    void Scene::OnRuntimeStop()
    {
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
    }

    /*
     * TODO: Very late in dev split Scene into editor scene and runtime scene to allow for
     * TODO: different behavior and rendering settings
     */
    
    void Scene::OnUpdateRuntime(float DeltaTime)
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
                Renderer2D::Draw::Sprite(transform.GetTransform(), sprite, (int)entity);
                
                // Renderer2D::Draw::Quad(transform.GetTransform(), sprite.Color);
            }

            Renderer2D::EndScene();
        }
    }
    
    void Scene::OnUpdateEditor(float DeltaTime, EditorCamera& camera)
    {
        Renderer2D::BeginScene(camera);
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            // Renderer2D::Draw::Quad(transform.GetTransform(), sprite.Color);
            Renderer2D::Draw::Sprite(transform.GetTransform(), sprite, (int)entity);
        }

        Renderer2D::EndScene();
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
    
    SceneObject Scene::GetMainCameraSO()
    {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& camera = view.get<CameraComponent>(entity);
            if (camera.IsMain)
            {
                return SceneObject{entity, this};
            }
        }
        return {};
    }

    SceneObject Scene::GetSceneObject(entt::entity entity)
    {
        return SceneObject{entity, this};
    }


    template <>
    void Scene::OnComponentAdded(SceneObject& entity, TagComponent& component)
    {
    }
    
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, TransformComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<CameraComponent>(SceneObject& entity, CameraComponent& component)
    {
        if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
            component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }
    
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, SpriteRendererComponent& component)
    {
    }
    
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, NativeScriptComponent& component)
    {
    }
    // TODO: add & not focused to selection
    // TODO call Time.deltatime directly
}

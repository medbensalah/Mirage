#include "MrgPch.h"

#include "Scene.h"
#include "SceneObject.h"
#include "ScriptableSceneObject.h"
#include "Components/Base/TransformComponent.h"
#include "Components/Base/TagComponent.h"
#include "Components/Physics/RigidBody2DComponent.h"
#include "Components/Physics/BoxCollider2DComponent.h"
#include "Components/Rendering/SpriteRendererComponent.h"
#include "Components/Rendering/CameraComponent.h"
#include "Components/NativeScriptComponent.h"
#include "Mirage/Renderer/Renderer2D.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "Mirage/Physics/Globals.h"

namespace Mirage
{
	static b2BodyType MrgToB2DBodyType(RigidBody2DComponent::BodyType type)
	{
		switch (type)
		{
		case RigidBody2DComponent::BodyType::Static: return b2_staticBody;
		case RigidBody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		case RigidBody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
		}

		MRG_CORE_ASSERT(false, "Unknown body type!!!");
		return b2_staticBody;
	}
	
	Scene::Scene()
	{
		
	}
	
	Scene::~Scene()
	{
		for (auto& entity : m_Hierarchy)
		{
			m_Registry.destroy(entity.first);
		}
	}

	template <typename Component>
	static void CopyComponents(entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<UUID, entt::entity>& map)
	{
		auto view = srcRegistry.view<Component>();
		for (auto e : view)
		{
			UUID uuid = srcRegistry.get<IDComponent>(e).ID;
			entt::entity dstEntityID = map.at(uuid);

			auto& component = srcRegistry.get<Component>(e);
			dstRegistry.emplace_or_replace<Component>(dstEntityID, component);
		}
	}

	Ref<Scene> Scene::Copy(const Ref<Scene> source)
	{		
		Ref<Scene> scene = CreateRef<Scene>();


		scene->m_ViewportWidth = source->m_ViewportWidth;
		scene->m_ViewportHeight = source->m_ViewportHeight;

		auto& srcReg = source->m_Registry;
		auto& dstReg = scene->m_Registry;
		std::unordered_map<UUID, entt::entity> entt_UUID_Map;

		auto idView = srcReg.view<IDComponent>();
		for (auto entity : idView)
		{
			UUID id = idView.get<IDComponent>(entity).ID;
			const auto& name = srcReg.get<TagComponent>(entity).Tag;

			SceneObject newSO = scene->CreateSceneObjectWithUUID(id, name);
			entt_UUID_Map[id] = (entt::entity)newSO;
		}

		for (auto& kv : source->m_Hierarchy)
		{
			SceneObject oldSO = {kv.first, source.get()};
			if (SceneObject parent = oldSO.GetParent())
			{
				SceneObject newParent = {entt_UUID_Map[parent.GetUUID()], scene.get()};
				newParent.AddChild(entt_UUID_Map[oldSO.GetUUID()]);
			}
		}
		
		CopyComponents<TransformComponent>(dstReg, srcReg,  entt_UUID_Map);
		CopyComponents<SpriteRendererComponent>(dstReg, srcReg,  entt_UUID_Map);
		CopyComponents<CameraComponent>(dstReg, srcReg,  entt_UUID_Map);
		CopyComponents<RigidBody2DComponent>(dstReg, srcReg,  entt_UUID_Map);
		CopyComponents<BoxCollider2DComponent>(dstReg, srcReg,  entt_UUID_Map);
		CopyComponents<NativeScriptComponent>(dstReg, srcReg,  entt_UUID_Map);

		return scene;
	}

	SceneObject Scene::CreateSceneObject(const std::string& name)
    {
        return CreateSceneObjectWithUUID(UUID(), name);
    }

    SceneObject Scene::CreateSceneObjectWithUUID(UUID uuid, const std::string& name)
    {        
        SceneObject sceneObject = {m_Registry.create(), this};
        Relationship r;
        m_Hierarchy.emplace(sceneObject, r);
		sceneObject.AddComponent<IDComponent>(uuid);
        auto& tag = sceneObject.AddComponent<TagComponent>();
        auto& transform = sceneObject.AddComponent<TransformComponent>(this);
        tag.Tag = name.empty() ? "Entity" : name;
        
        return sceneObject;
    }

    SceneObject Scene::CreateChildSceneObject(entt::entity parent, const std::string& name)
    {
        return CreateChildSceneObjectWithUUID(UUID(), parent, name);
    }

    SceneObject Scene::CreateChildSceneObjectWithUUID(UUID uuid, entt::entity parent, const std::string& name)
	{
		SceneObject so = CreateSceneObjectWithUUID(uuid, name);

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
		m_PhysicsWorld = new b2World(b2Vec2(0.0f, Physics2D::Gravity));
		
		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto e : view)
		{
			SceneObject so = {e, this};
			auto& transform = so.GetComponent<TransformComponent>();
			auto& rb = so.GetComponent<RigidBody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.position.Set(transform.Position().x, transform.Position().y);
			bodyDef.angle = Radians(transform.Rotation().z);
			
			bodyDef.type = MrgToB2DBodyType(rb.Type);
			bodyDef.gravityScale = rb.GravityScale;
			bodyDef.fixedRotation = rb.FixedRotation;
			
			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			rb.RuntimeBody = body;

			// TODO : add istrigger (issensro)
			
			if (so.HasComponent<BoxCollider2DComponent>())
			{
				auto& collider = so.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape shape;
				shape.SetAsBox(collider.Size.x * transform.Scale().x, collider.Size.y * transform.Scale().y,
				               {collider.Offset.x, collider.Offset.y}, 0);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &shape;
				fixtureDef.density = collider.Density;
				fixtureDef.friction = collider.Friction;
				fixtureDef.restitution = collider.Bounciness;
				fixtureDef.restitutionThreshold = collider.BouncinessThreshold;
				
				body->CreateFixture(&fixtureDef);
			}
		}

		m_PhysicsTimer.Reset();
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
        // ---------------- Update scripts ----------------
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

        // -------------------- Physics --------------------

        {
	        if (Physics2D::UpdateEveryFrame)
	        {
		        m_PhysicsWorld->Step(DeltaTime, Physics2D::VelocityIterations, Physics2D::PositionIterations);
		        // update transform component from physics
		        auto view = m_Registry.view<RigidBody2DComponent>();
		        for (auto e : view)
		        {
			        SceneObject so = {e, this};
			        auto& transform = so.GetComponent<TransformComponent>();
			        auto& rb = so.GetComponent<RigidBody2DComponent>();

			        if (rb.Type == RigidBody2DComponent::BodyType::Static) continue;
			        const auto& position = rb.RuntimeBody->GetPosition();
			        transform.SetPosition({position.x, position.y, transform.Position().z});
			        transform.SetRotation({
				        transform.Rotation().x, transform.Rotation().y, Degrees(rb.RuntimeBody->GetAngle())
			        });
		        }
		        m_PhysicsTimer.Reset();
	        }
	        else
	        {
		        if (m_PhysicsTimer.Elapsed() >= Physics2D::TimeStep)
		        {
			        m_PhysicsWorld->Step(m_PhysicsTimer.Elapsed(), Physics2D::VelocityIterations,
			                             Physics2D::PositionIterations);
			        // update transform component from physics
			        auto view = m_Registry.view<RigidBody2DComponent>();
			        for (auto e : view)
			        {
				        SceneObject so = {e, this};
				        auto& transform = so.GetComponent<TransformComponent>();
				        auto& rb = so.GetComponent<RigidBody2DComponent>();

				        if (rb.Type == RigidBody2DComponent::BodyType::Static) continue;
				        const auto& position = rb.RuntimeBody->GetPosition();
				        transform.SetPosition({position.x, position.y, transform.Position().z});
				        transform.SetRotation({
					        transform.Rotation().x, transform.Rotation().y, Degrees(rb.RuntimeBody->GetAngle())
				        });
			        }
			        m_PhysicsTimer.Reset();
		        }
	        }
        }
        // -------------------- Render2D --------------------
        RenderRuntime(DeltaTime);
    }

    void Scene::RenderRuntime(float DeltaTime)
    {
	    Camera* mainCamera = nullptr;
	    Mat4 cameraTransform;
	    {
		    auto group = m_Registry.group(entt::get<TransformComponent, CameraComponent>);
		    for (auto entity : group)
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
	    if (mainCamera)
	    {
		    Renderer2D::BeginScene(*mainCamera, cameraTransform);
		    auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		    for (auto entity : group)
		    {
			    auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			    Renderer2D::Draw::Sprite(transform.GetTransform(), sprite, (int)entity);
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


	template <typename T>
	void Scene::OnComponentAdded(SceneObject& entity, T& component)
	{
	}
	
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, TagComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded(SceneObject& entity, IDComponent& component)
    {
    }
    
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, TransformComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded(SceneObject& entity, CameraComponent& component)
    {
        if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
            component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }
    
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, SpriteRendererComponent& component)
    {
    }
    
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, RigidBody2DComponent& component)
    {
    }
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, BoxCollider2DComponent& component)
    {
    }
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, NativeScriptComponent& component)
    {
    }
    // TODO: add & not focused to selection
}

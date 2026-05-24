#include "MrgPch.h"

#include "Scene.h"
#include "SceneObject.h"
#include "ScriptableSceneObject.h"
#include "Components/Base/TransformComponent.h"
#include "Components/Base/TagComponent.h"
#include "Components/Physics/Rigidbody2DComponent.h"
#include "Components/Physics/BoxCollider2DComponent.h"
#include "Components/Physics/CircleCollider2DComponent.h"
#include "Components/Rendering/SpriteRendererComponent.h"
#include "Components/Rendering/CameraComponent.h"
#include "Components/NativeScriptComponent.h"
#include "Components/ScriptComponent.h"
#include "Mirage/Renderer/Renderer2D.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"
#include "Components/AllComponents.h"
#include "Components/Base/HierarchyComponent.h"
#include "Mirage/Definitions/Physics.h"
#include "Mirage/Scripting/ScriptingEngine.h"

namespace Mirage
{
	static b2BodyType MrgToB2DBodyType(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
		case Rigidbody2DComponent::BodyType::Static: return b2_staticBody;
		case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		case Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
		}

		MRG_CORE_ASSERT(false, "Unknown body type!!!");
		return b2_staticBody;
	}
	
	Scene::Scene()
	{
		
	}
	
	Scene::~Scene()
	{
		delete m_PhysicsWorld;
	}

	template <typename... Component>
	static void CopyComponent(entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<UUID, entt::entity>& map)
	{
		// auto view = srcRegistry.view<Component>();
		// for (auto e : view)
		// {
		// 	UUID uuid = srcRegistry.get<IDComponent>(e).ID;
		// 	entt::entity dstEntityID = map.at(uuid);
		//
		// 	auto& component = srcRegistry.get<Component>(e);
		// 	dstRegistry.emplace_or_replace<Component>(dstEntityID, component);
		// }
		([&]()
		{
			auto view = srcRegistry.view<Component>();
			for (auto srcEntity : view)
			{
				entt::entity dstEntity = map.at(srcRegistry.get<IDComponent>(srcEntity).ID);
				auto& srcComponent = srcRegistry.get<Component>(srcEntity);
				dstRegistry.emplace_or_replace<Component>(dstEntity, srcComponent);
			}
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	static SceneObject CopySceneObject(SceneObject so, Ref<Scene> dst, Ref<Scene> src, std::unordered_map<UUID, entt::entity>* map)
	{
		UUID id = so.GetUUID();
		const auto& name = so.GetName();

		SceneObject newSO = dst->CreateSceneObjectWithUUID(id, name);
		(*map)[id] = (entt::entity)newSO;

		TransformComponent& newTransform = newSO.GetComponent<TransformComponent>();
		TransformComponent& transform = so.GetComponent<TransformComponent>();

		newTransform.Copy(transform);
	
		for (auto& child : so.GetChildren())
		{
			SceneObject c = CopySceneObject({child, src.get()}, dst, src, map);
			newSO.AddChild(c);
			c.SetParent(newSO);
		}
		return newSO;
	}
	//
	// template <typename Component>
	// static void CopyComponentIfExists(SceneObject dst, SceneObject src)
	// {
	// 	if (src.HasComponent<Component>())
	// 	{
	// 		dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
	// 	}
	// }
	template<typename... Component>
		static void CopyComponentIfExists(SceneObject dst, SceneObject src)
	{
		([&]()
		{
			if (src.HasComponent<Component>())
				dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, SceneObject dst, SceneObject src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}
    
	// SceneObject Scene::DuplicateSceneObject(SceneObject entity)
	// {
	// 	SceneObject duplicate = CreateSceneObject(entity.GetName());
	//
	// 	if (entity.HasParent())
	// 	{
	// 		duplicate.SetParent(entity.GetParent());
	// 	}
	// 	
	// 	for (auto& child : entity.GetChildren())
	// 	{
	// 		SceneObject c = DuplicateSceneObject({child,entity.m_Scene});
	// 		c.SetParent(duplicate);
	// 	}
	// 	
	// 	duplicate.GetComponent<TransformComponent>().Copy(entity.GetComponent<TransformComponent>());
	// 	CopyComponentIfExists<BoxCollider2DComponent>(duplicate, entity);
	// 	CopyComponentIfExists<CircleCollider2DComponent>(duplicate, entity);
	// 	CopyComponentIfExists<RigidBody2DComponent>(duplicate, entity);
	// 	CopyComponentIfExists<CameraComponent>(duplicate, entity);
	// 	CopyComponentIfExists<SpriteRendererComponent>(duplicate, entity);
	// 	CopyComponentIfExists<CircleRendererComponent>(duplicate, entity);
	// 	CopyComponentIfExists<NativeScriptComponent>(duplicate, entity);
	// 	return duplicate;
	// }

	SceneObject Scene::DuplicateSceneObject(SceneObject entity)
	{
		SceneObject newEntity = CreateSceneObject(entity.GetName());
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
		return newEntity;
	}
	
	Ref<Scene> Scene::Copy(const Ref<Scene> source)
	{		
		Ref<Scene> scene = CreateRef<Scene>();


		scene->m_ViewportWidth = source->m_ViewportWidth;
		scene->m_ViewportHeight = source->m_ViewportHeight;

		auto& srcReg = source->m_Registry;
		auto& dstReg = scene->m_Registry;
		std::unordered_map<UUID, entt::entity> entt_UUID_Map;

		for (auto& h : source->m_Hierarchy)
		{
			CopySceneObject({h.second.m_entity, source.get()}, scene, source, &entt_UUID_Map);
		}
		
		

		// auto idView = srcReg.view<IDComponent>();
		// auto& rb = idView.rbegin();
		// auto& re = idView.rend();
		// for (auto it = rb; it != re; ++it)
		// {
		// 	SceneObject so = { *it, source.get() };
		// 	if (!so.HasParent())
		// 	{
		// 		UUID id = so.GetUUID();
		// 		const auto& name = so.GetTag();
		//
		// 		SceneObject newSO = scene->CreateSceneObjectWithUUID(id, name);
		// 		entt_UUID_Map[id] = (entt::entity)newSO;
		// 	}
		// }

		// auto idView = srcReg.view<IDComponent>();
		// for (auto entity : idView)
		// {
		// 	UUID id = idView.get<IDComponent>(entity).ID;
		// 	const auto& name = srcReg.get<TagComponent>(entity).Tag;
		//
		// 	SceneObject newSO = scene->CreateSceneObjectWithUUID(id, name);
		// 	entt_UUID_Map[id] = (entt::entity)newSO;
		// }

		// CopyComponents<TransformComponent>(dstReg, srcReg,  entt_UUID_Map);
		
		// CopyComponents<SpriteRendererComponent>(dstReg, srcReg,  entt_UUID_Map);
		// CopyComponents<CircleRendererComponent>(dstReg, srcReg,  entt_UUID_Map);
		// CopyComponents<CameraComponent>(dstReg, srcReg,  entt_UUID_Map);
		// CopyComponents<RigidBody2DComponent>(dstReg, srcReg,  entt_UUID_Map);
		// CopyComponents<BoxCollider2DComponent>(dstReg, srcReg,  entt_UUID_Map);
		// CopyComponents<CircleCollider2DComponent>(dstReg, srcReg,  entt_UUID_Map);
		// CopyComponents<NativeScriptComponent>(dstReg, srcReg,  entt_UUID_Map);
		
		CopyComponent(AllComponents{}, dstReg, srcReg, entt_UUID_Map);
		return scene;
	}

	SceneObject Scene::CreateSceneObject(const std::string& name)
    {
        return CreateSceneObjectWithUUID(UUID(), name);
    }

    SceneObject Scene::CreateSceneObjectWithUUID(UUID uuid, const std::string& name)
    {        
        SceneObject sceneObject = {m_Registry.create(), this};
		sceneObject.AddComponent<IDComponent>(uuid);
		sceneObject.AddComponent<HierarchyComponent>();

		auto& tag = sceneObject.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		
		auto& tr = sceneObject.AddComponent<TransformComponent>(this);
		// for some reason it does not work without this line
		tr.GetTransform();
		
		auto& hierarchy = sceneObject.GetComponent<HierarchyComponent>();
		hierarchy.m_Index = m_Hierarchy.size();
		m_Hierarchy[hierarchy.m_Index] = hierarchy;

		m_entt_uuid_map[uuid] = sceneObject;
		
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
		m_entt_uuid_map.erase(entity.GetUUID());
        entity.Destroy();
    }

    void Scene::OnRuntimeStart()
    {
		OnPhysics2DInit();

		//scripts
		{
			ScriptingEngine::OnRuntimeStart(this);
			//instantiae all script behaviors
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				SceneObject so = {e, this};
				ScriptingEngine::OnCreateBehavior(so);
			}
		}
    }

    void Scene::OnRuntimeStop()
    {
	    OnPhysics2DStop();

		//scripts
		ScriptingEngine::OnRuntimeStop();
    }

    void Scene::OnSimulationStart()
    {
	    OnPhysics2DInit();
    }

    void Scene::OnSimulationStop()
    {
	    OnPhysics2DStop();
    }

    /*
     * TODO: Very late in dev split Scene into editor scene and runtime scene to allow for
     * TODO: different behavior and rendering settings
     */
    
    void Scene::OnUpdateRuntime(float DeltaTime)
    {
        // ---------------- Update scripts ----------------

        auto CSview = m_Registry.view<ScriptComponent>();
        //scripts
        // C# behavior OnUpdate
        for (auto e : CSview)
        {
	        SceneObject so = {e, this};
	        ScriptingEngine::OnUpdateBehavior(so, DeltaTime);
        }
/*
		// native scripts
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
*/
        // -------------------- Physics --------------------

        {
	        if (Physics2D::UpdateEveryFrame)
	        {
		        m_PhysicsWorld->Step(DeltaTime, Physics2D::VelocityIterations, Physics2D::PositionIterations);
		        // update transform component from physics
		        auto view = m_Registry.view<Rigidbody2DComponent>();
		        for (auto e : view)
		        {
			        SceneObject so = {e, this};
			        auto& transform = so.GetComponent<TransformComponent>();
			        auto& rb = so.GetComponent<Rigidbody2DComponent>();

			        if (rb.Type == Rigidbody2DComponent::BodyType::Static) continue;
			        const auto& position = rb.RuntimeBody->GetPosition();
			        transform.SetPosition({position.x, position.y, transform.Position().z});
			        transform.SetRotation({
				        transform.Rotation().x, transform.Rotation().y, Degrees(rb.RuntimeBody->GetAngle())
			        });
		        }
	        	
	        	for (auto e : CSview)
	        	{
	        		SceneObject so = {e, this};
	        		ScriptingEngine::OnPhysicsUpdateBehavior(so, DeltaTime);
	        	}
		        m_PhysicsTimer.Reset();
	        }
	        else
	        {
		        if (m_PhysicsTimer.Elapsed() >= (1.0f/Physics2D::FPS))
		        {
			        m_PhysicsWorld->Step(m_PhysicsTimer.Elapsed(), Physics2D::VelocityIterations,
			                             Physics2D::PositionIterations);
			        // update transform component from physics
			        auto view = m_Registry.view<Rigidbody2DComponent>();
			        for (auto e : view)
			        {
				        SceneObject so = {e, this};
				        auto& transform = so.GetComponent<TransformComponent>();
				        auto& rb = so.GetComponent<Rigidbody2DComponent>();

				        if (rb.Type == Rigidbody2DComponent::BodyType::Static) continue;
				        const auto& position = rb.RuntimeBody->GetPosition();
				        transform.SetPosition({position.x, position.y, transform.Position().z});
				        transform.SetRotation({
					        transform.Rotation().x, transform.Rotation().y, Degrees(rb.RuntimeBody->GetAngle())
				        });
			        }
		        	for (auto e : CSview)
		        	{
		        		SceneObject so = {e, this};
		        		ScriptingEngine::OnPhysicsUpdateBehavior(so, m_PhysicsTimer.Elapsed());
		        	}
			        m_PhysicsTimer.Reset();
		        }
	        }
        }
        // -------------------- Render2D --------------------
		RenderRuntime();
    }


    void Scene::RenderRuntime()
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

		    {
			    auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			    for (auto entity : group)
			    {
				    auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				    // Renderer2D::Draw::Quad(transform.GetTransform(), sprite.Color);
				    Renderer2D::Draw::Sprite(transform.GetTransform(), sprite, (int)entity);
			    }
		    }
		    {
			    auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();

			    for (auto entity : view)
			    {
				    auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
				    // Renderer2D::Draw::Quad(transform.GetTransform(), sprite.Color);
				    Renderer2D::Draw::Circle(transform.GetTransform(), circle, (int)entity);
			    }
		    }
		    Renderer2D::EndScene();
	    }
    }
	
    void Scene::OnUpdateSimulation(float DeltaTime, EditorCamera& camera)
    {
		// Physics
		{
			if (Physics2D::UpdateEveryFrame)
			{
				m_PhysicsWorld->Step(DeltaTime, Physics2D::VelocityIterations, Physics2D::PositionIterations);
				// update transform component from physics
				auto view = m_Registry.view<Rigidbody2DComponent>();
				for (auto e : view)
				{
					SceneObject so = {e, this};
					auto& transform = so.GetComponent<TransformComponent>();
					auto& rb = so.GetComponent<Rigidbody2DComponent>();

					if (rb.Type == Rigidbody2DComponent::BodyType::Static) continue;
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
				if (m_PhysicsTimer.Elapsed() >= (1.0f/Physics2D::FPS))
				{
					m_PhysicsWorld->Step(m_PhysicsTimer.Elapsed(), Physics2D::VelocityIterations,
										 Physics2D::PositionIterations);
					// update transform component from physics
					auto view = m_Registry.view<Rigidbody2DComponent>();
					for (auto e : view)
					{
						SceneObject so = {e, this};
						auto& transform = so.GetComponent<TransformComponent>();
						auto& rb = so.GetComponent<Rigidbody2DComponent>();

						if (rb.Type == Rigidbody2DComponent::BodyType::Static) continue;
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

		//Render
       // RenderScene(camera);
		RenderRuntime();
    }
	
    void Scene::OnUpdateEditor(float DeltaTime, EditorCamera& camera)
    {
       RenderScene(camera);
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

    SceneObject Scene::GetSceneObjectByUUID(UUID uuid)
    {
	    if (m_entt_uuid_map.find(uuid) != m_entt_uuid_map.end())
		    return SceneObject{m_entt_uuid_map[uuid], this};

		return {};
    }

    void Scene::OnPhysics2DInit()
    {
		m_PhysicsWorld = new b2World(b2Vec2(0.0f, Physics2D::Gravity));
		MRG_CORE_INFO("gravity {0}", Physics2D::Gravity);
		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			SceneObject so = {e, this};
			auto& transform = so.GetComponent<TransformComponent>();
			auto& rb = so.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.position.Set(transform.Position().x, transform.Position().y);
			bodyDef.angle = Radians(transform.Rotation().z);
			
			bodyDef.type = MrgToB2DBodyType(rb.Type);
			bodyDef.gravityScale = rb.GravityScale;
			bodyDef.fixedRotation = rb.FixedRotation;
			
			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			rb.RuntimeBody = body;
			// TODO : add istrigger (issensor)
			
			if (so.HasComponent<BoxCollider2DComponent>())
			{
				auto& boxCollider2D = so.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape shape;
				shape.SetAsBox(boxCollider2D.Size.x * transform.Scale().x, boxCollider2D.Size.y * transform.Scale().y,
				               {boxCollider2D.Offset.x, boxCollider2D.Offset.y}, 0);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &shape;
				fixtureDef.density = boxCollider2D.Density;
				fixtureDef.friction = boxCollider2D.Friction;
				fixtureDef.restitution = boxCollider2D.Bounciness;
				fixtureDef.restitutionThreshold = boxCollider2D.BouncinessThreshold;
				
				body->CreateFixture(&fixtureDef);
			}
			
			if (so.HasComponent<CircleCollider2DComponent>())
			{
				auto& circleCollider2D = so.GetComponent<CircleCollider2DComponent>();

				b2CircleShape shape;
				shape.m_p.Set(circleCollider2D.Offset.x, circleCollider2D.Offset.y);
				shape.m_radius = transform.Scale().x * circleCollider2D.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &shape;
				fixtureDef.density = circleCollider2D.Density;
				fixtureDef.friction = circleCollider2D.Friction;
				fixtureDef.restitution = circleCollider2D.Bounciness;
				fixtureDef.restitutionThreshold = circleCollider2D.BouncinessThreshold;
				
				body->CreateFixture(&fixtureDef);
			}
		}

		m_PhysicsTimer.Reset();
    }

    void Scene::OnPhysics2DStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
    }

    void Scene::RenderScene(EditorCamera& camera)
    {
	    Renderer2D::BeginScene(camera);
	    {
		    auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		    for (auto entity : group)
		    {
			    auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			    Renderer2D::Draw::Sprite(transform.GetTransform(), sprite, (int)entity);
		    }
	    }
	    {
		    auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();

		    for (auto entity : view)
		    {
			    auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
			    Renderer2D::Draw::Circle(transform.GetTransform(), circle, (int)entity);
		    }
	    }
	    Renderer2D::EndScene();
    }


    template <typename T>
	void Scene::OnComponentAdded(SceneObject& entity, T& component)
	{
		static_assert(sizeof(T) == 0);
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
    void Scene::OnComponentAdded(SceneObject& entity, HierarchyComponent& component)
    {
		component.m_entity = entity.m_Entity;
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
    void Scene::OnComponentAdded(SceneObject& entity, CircleRendererComponent& component)
    {
    }
    
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, Rigidbody2DComponent& component)
    {
    }
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, BoxCollider2DComponent& component)
    {
    }
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, CircleCollider2DComponent& component)
    {
    }
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, NativeScriptComponent& component)
    {
    }
    template <>
    void Scene::OnComponentAdded(SceneObject& entity, ScriptComponent& component)
    {
    }
	
    // TODO: add & not focused to selection
}

#include "MrgPch.h"
#include "SceneSerializer.h"

#include <yaml-cpp/yaml.h>

#include "SceneObject.h"
#include "Components/Rendering/CameraComponent.h"
#include "Components/Rendering/SpriteRendererComponent.h"
#include "Components/Base/TransformComponent.h"
#include "Components/Physics/BoxCollider2DComponent.h"
#include "Components/Physics/RigidBody2DComponent.h"

namespace YAML
{
    template <>
    struct convert<Vec2>
    {
        static Node encode(const Vec2& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, Vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template <>
    struct convert<Vec3>
    {
        static Node encode(const Vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, Vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template <>
    struct convert<Vec4>
    {
        static Node encode(const Vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, Vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
}

namespace Mirage
{
	std::string RigidBody2DTypeToString(RigidBody2DComponent::BodyType type)
	{
		switch (type)
		{
		case RigidBody2DComponent::BodyType::Static:
			return "Static";
		case RigidBody2DComponent::BodyType::Kinematic:
			return "Kinematic";
		case RigidBody2DComponent::BodyType::Dynamic:
			return "Dynamic";
		}
		MRG_CORE_WARN("Unknown body type while serializing, falling back to static body");
		return "Static";
	}
	RigidBody2DComponent::BodyType RigidBody2DTypeFromString(std::string type)
	{
		if (type == "Static")
		{
			return RigidBody2DComponent::BodyType::Static;
		}
		if (type == "Kinematic")
		{
			return RigidBody2DComponent::BodyType::Kinematic;
		}
		if (type == "Dynamic")
		{
			return RigidBody2DComponent::BodyType::Dynamic;
		}
		MRG_CORE_WARN("Unknown body type while deserializing, falling back to static body");
		return RigidBody2DComponent::BodyType::Static;
	}
	
    YAML::Emitter& operator<<(YAML::Emitter& out, const Vec2& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const Vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const Vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }
    
    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
        : m_Scene(scene)
    {
    }

    static void SerializeSceneObject(YAML::Emitter& out, SceneObject so)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Entity" << YAML::Value << so.GetEntity();

        if (so.HasComponent<TagComponent>())
        {
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap;

            auto& tag = so.GetComponent<TagComponent>().Tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;

            out << YAML::EndMap;
        }
        if (so.HasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap;
            
            auto& tc = so.GetComponent<TransformComponent>();
            out << YAML::Key << "Position" << YAML::Value << tc.Position();
            out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation();
            out << YAML::Key << "Scale" << YAML::Value << tc.Scale();
            
            out << YAML::EndMap;
        }
        if (so.HasComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap;

            auto& cameraComponent = so.GetComponent<CameraComponent>();
            auto& camera = cameraComponent.Camera;
            
            out << YAML::Key << "Camera" << YAML::Value;
            out << YAML::BeginMap;
            out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
            out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVFOV();
            out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
            out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
            out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
            out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
            out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
            out << YAML::EndMap;

            out << YAML::Key << "Primary" << YAML::Value << cameraComponent.IsMain;
            out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

            out << YAML::EndMap;
        }
        if (so.HasComponent<SpriteRendererComponent>())
        {
            out << YAML::Key << "SpriteRendererComponent";
            out << YAML::BeginMap;

            auto& src = so.GetComponent<SpriteRendererComponent>();
            out << YAML::Key << "Color" << YAML::Value << src.Color;
            out << YAML::Key << "Texture" << YAML::Value << src.Texture->GetPath().string();
            out << YAML::Key << "Tiling" << YAML::Value << src.Tiling;
            out << YAML::Key << "Offset" << YAML::Value << src.Offset;

            out << YAML::EndMap;
        }
        if (so.HasComponent<RigidBody2DComponent>())
        {
            out << YAML::Key << "RigidBody2DComponent";
            out << YAML::BeginMap;

            auto& rb2d = so.GetComponent<RigidBody2DComponent>();
            out << YAML::Key << "Body type" << YAML::Value << RigidBody2DTypeToString(rb2d.Type);
            out << YAML::Key << "Gravity scale" << YAML::Value << rb2d.GravityScale;
            out << YAML::Key << "Fixed rotation" << YAML::Value << rb2d.FixedRotation;

            out << YAML::EndMap;
        }
        if (so.HasComponent<BoxCollider2DComponent>())
        {
            out << YAML::Key << "BoxCollider2DComponent";
            out << YAML::BeginMap;

            auto& collider = so.GetComponent<BoxCollider2DComponent>();
            out << YAML::Key << "Size" << YAML::Value << collider.Size;
            out << YAML::Key << "Offset" << YAML::Value << collider.Offset;
            out << YAML::Key << "Density" << YAML::Value << collider.Density;
            out << YAML::Key << "Friction" << YAML::Value << collider.Friction;
            out << YAML::Key << "Bounciness" << YAML::Value << collider.Bounciness;
            out << YAML::Key << "BouncinessThreshold" << YAML::Value << collider.BouncinessThreshold;

            out << YAML::EndMap;
        }

#pragma region Children
        if (so.GetChildCount() == 0)
        {
            out << YAML::EndMap;
            return;
        }

        auto& children = so.GetChildren();

        out << YAML::Key << "Children" << YAML::Value << YAML::BeginSeq;
        for (auto& entity : children)
        {
            SceneObject child = {entity, so.GetScene()};
            SerializeSceneObject(out, child);
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;
#pragma endregion 
    }
	
    static bool DeserializeSceneObject(YAML::detail::iterator_value& entity, SceneObject& so)
    {
        auto transformComponent = entity["TransformComponent"];
        if (transformComponent)
        {
            // Entities always have transforms
            auto& tc = so.GetComponent<TransformComponent>();
            tc.SetPosition(transformComponent["Position"].as<Vec3>());
            tc.SetRotation(transformComponent["Rotation"].as<Vec3>());
            tc.SetScale(transformComponent["Scale"].as<Vec3>());
        }

        auto cameraComponent = entity["CameraComponent"];
        if (cameraComponent)
        {
            auto& cc = so.AddComponent<CameraComponent>();
            auto& cameraProps = cameraComponent["Camera"];
            cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
            
            cc.Camera.SetPerspectiveVFOV(cameraProps["PerspectiveFOV"].as<float>());
            cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
            cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());
            
            cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
            cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
            cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());
            
            cc.IsMain = cameraComponent["Primary"].as<bool>();
            cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
        }

        auto spriteRendererComponent = entity["SpriteRendererComponent"];
        if (spriteRendererComponent)
        {
            auto& src = so.AddComponent<SpriteRendererComponent>();
            src.Color = spriteRendererComponent["Color"].as<Vec4>();
		    src.Texture = Texture2D::Create(spriteRendererComponent["Texture"].as<std::string>());
        	src.Tiling = spriteRendererComponent["Tiling"].as<Vec2>();
        	src.Offset = spriteRendererComponent["Offset"].as<Vec2>();
        }
		
        auto rigidBody2DComponent = entity["RigidBody2DComponent"];
        if (rigidBody2DComponent)
        {
            auto& rb2d = so.AddComponent<RigidBody2DComponent>();
            rb2d.Type = RigidBody2DTypeFromString(rigidBody2DComponent["Body type"].as<std::string>());
        	rb2d.GravityScale = rigidBody2DComponent["Gravity scale"].as<float>();
        	rb2d.FixedRotation = rigidBody2DComponent["Fixed rotation"].as<bool>();
        }
		
        auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
        if (boxCollider2DComponent)
        {
            auto& collider = so.AddComponent<BoxCollider2DComponent>();
        	collider.Size = boxCollider2DComponent["Size"].as<Vec2>();
        	collider.Offset = boxCollider2DComponent["Offset"].as<Vec2>();
        	collider.Density = boxCollider2DComponent["Density"].as<float>();
        	collider.Friction = boxCollider2DComponent["Friction"].as<float>();
        	collider.Bounciness = boxCollider2DComponent["Bounciness"].as<float>();
        	collider.BouncinessThreshold = boxCollider2DComponent["BouncinessThreshold"].as<float>();
        }

#pragma region Children
        auto children = entity["Children"];
        if (children)
        {
            for (auto& child : children)
            {

                uint32_t uuid = child["Entity"].as<uint64_t>();
                MRG_CORE_INFO("Deserializing entity with ID = {0}", uuid);
    
                std::string tag;
                auto tagComponent = child["TagComponent"];
                if (tagComponent)
                    tag = tagComponent["Tag"].as<std::string>();
                
                SceneObject childSo = so.GetScene()->CreateChildSceneObject(so, tag);
                
                DeserializeSceneObject(child, childSo);
            }
        }
#pragma endregion 
        return true;
    }
    
    void SceneSerializer::SerializeText(const std::filesystem::path& filepath)
    {
        YAML::Emitter out;

        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        for(auto& kv : m_Scene->m_Hierarchy)
        {
            if(kv.second.m_Parent == entt::null)
            {
                MRG_CORE_INFO("Serializing entity with ID = {0}", (uint32_t)kv.first);
                SceneObject so = { kv.first, m_Scene.get() };
                SerializeSceneObject(out, so);
            }
        }
        out << YAML::EndSeq;
        
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    bool SceneSerializer::DeserializeText(const std::filesystem::path& filepath)
    {
        bool result = true;
    	YAML::Node data;
    	try
    	{
    		data = YAML::LoadFile(filepath.string());
    	}
    	catch (YAML::ParserException e)
    	{
    		return false;
    	}
        if (!data["Scene"])
            return false;

        std::string sceneName = data["Scene"].as<std::string>();
        MRG_CORE_INFO("Deserializing scene '{0}'", sceneName);

        auto entities = data["Entities"];
        if (entities)
        {
           for (auto entity : entities)
           {                
               uint32_t uuid = entity["Entity"].as<uint64_t>();
               MRG_CORE_INFO("Deserializing entity with ID = {0}", uuid);
    
               std::string tag;
               auto tagComponent = entity["TagComponent"];
               if (tagComponent)
                   tag = tagComponent["Tag"].as<std::string>();
    
               SceneObject so = m_Scene->CreateSceneObject(tag);
               result &= DeserializeSceneObject(entity, so);
           }
        }
    }

    void SceneSerializer::SerializeBinary(const std::filesystem::path& filepath)
    {
        MRG_CORE_ASSERT(false, "Not implemented!");
    }

    bool SceneSerializer::DeserializeBinary(const std::filesystem::path& filepath)
    {
        MRG_CORE_ASSERT(false, "Not implemented!");
        return false;
    }
}

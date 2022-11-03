#include "MrgPch.h"
#include "SceneSerializer.h"

#include <yaml-cpp/yaml.h>

#include "SceneObject.h"
#include "Components/CameraComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/TransformComponent.h"

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
            // out << YAML::Key << "Texture" << YAML::Value << src.TexturePath;

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
            // src.TexturePath = spriteRendererComponent["Texture"].as<std::string>();
        }

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

        return true;
    }
    
    void SceneSerializer::SerializeText(const std::string& filepath)
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
                MRG_CORE_INFO("*");
                SerializeSceneObject(out, so);
                MRG_CORE_INFO("finished entity with ID = {0}", (uint32_t)kv.first);
            }
        }
        out << YAML::EndSeq;
        
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    bool SceneSerializer::DeserializeText(const std::string& filepath)
    {
        bool result = true;
        std::ifstream stream(filepath);
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());
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

    void SceneSerializer::SerializeBinary(const std::string& filepath)
    {
        MRG_CORE_ASSERT(false, "Not implemented!");
    }

    bool SceneSerializer::DeserializeBinary(const std::string& filepath)
    {
        MRG_CORE_ASSERT(false, "Not implemented!");
        return false;
    }
}

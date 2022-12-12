#pragma once

#include "glm/gtx/euler_angles.hpp"
#include "Mirage/Math/glmTypes.h"
#include "Mirage/ECS/Scene.h"

namespace Mirage
{
    class SceneObject;
    struct TransformComponent
    {
        TransformComponent() = default;
        TransformComponent(const Vec3& position)
            : m_Position(position)
        {
        }

        TransformComponent(Scene* scene)
            : m_Scene(scene)
        {
        }

    	void Copy(const TransformComponent& other)
        {
        	SetPosition(other.Position());
        	SetRotation(other.Rotation());
        	SetScale(other.Scale());
        }
        
        Mat4 GetLocalTransform();
        Mat4 GetTransform();

        SceneObject GetOwner();
        SceneObject GetOwner() const;

        void MarkPositionDirty();
        void MarkRotationDirty();
        void MarkScaleDirty();

        inline Vec3 Position() const { return m_Position; }
        inline Vec3 Rotation() const { return m_Rotation; }
        inline Quat RotationQ() const { return m_RotationQ; }
        inline Vec3 Scale() const { return m_Scale; }

        Vec3 WorldToLocalPosition(const Vec3& worldPosition);

        
        void SetPosition(const Vec3& position);
        void SetWorldPosition(const Vec3& position);

        void SetRotation(const Vec3& rotation);
        void SetWorldRotation(const Vec3& rotation);
        
        void SetScale(const Vec3& scale);
        void SetWorldScale(const Vec3& scale);

        void Translate(const Vec3& translation);
        void Rotate(const Vec3& rotation);
        void Scale(const Vec3& scale);
        
        Vec3 WorldPosition();
        Vec3 WorldRotation();
        Quat WorldRotationQ();
        Vec3 WorldScale();
        
    private:
        void SetRotation(const Quat& rotation);
        void SetWorldRotation(const Quat& rotation);
        void Rotate(const Quat& rotation);

    private:
        entt::entity m_Owner = {entt::null};
        Scene* m_Scene = nullptr;

        bool m_IsWorldPositionDirty = true;
        bool m_IsWorldRotationDirty = true;
        bool m_IsWorldScaleDirty = true;
        bool m_IsTransformDirty = true;

        Vec3 m_Position{0.0f, 0.0f, 0.0f};
        Vec3 m_Rotation{0.0f, 0.0f, 0.0f};
        Quat m_RotationQ{1.0f, 0.0f, 0.0f, 0.0f};
        Vec3 m_Scale{1.0f, 1.0f, 1.0f};

        Vec3 m_WorldPosition{0.0f, 0.0f, 0.0f};
        Vec3 m_WorldRotation{0.0f, 0.0f, 0.0f};
        Quat m_WorldRotationQ{1.0f, 0.0f, 0.0f, 0.0f};
        Vec3 m_WorldScale{1.0f, 1.0f, 1.0f};

        Mat4 m_transform{1.0f};
    };
}

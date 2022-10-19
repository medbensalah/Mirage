#pragma once
#include "TagComponent.h"
#include "Mirage/Core/Math.h"
#include "Mirage/ECS/Scene.h"

namespace Mirage
{
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

        Mat4 GetWorldTransform()
        {
            if (m_IsTransformDirty)
            {
                Mat4 rotation = glm::rotate(Mat4(1.0f), Radians(m_Rotation.x), Vec3(1.0f, 0.0f, 0.0f)) *
                    glm::rotate(Mat4(1.0f), Radians(m_Rotation.y), Vec3(0.0f, 1.0f, 0.0f)) *
                    glm::rotate(Mat4(1.0f), Radians(m_Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
                m_transform = glm::translate(Mat4(1.0f), m_Position) *
                    rotation *
                    glm::scale(Mat4(1.0f), m_Scale);

                SceneObject so{GetOwner(), m_Scene};
                if (so.HasParent())
                {
                    m_transform = so.GetParent().GetComponent<TransformComponent>().GetWorldTransform() * m_transform;
                }
            }

            m_IsTransformDirty = false;
            return m_transform;
        }

        SceneObject GetOwner()
        {
            if (m_Owner == entt::null)
                m_Owner = entt::to_entity<entt::entity, TransformComponent>(m_Scene->m_Registry, *this);

            SceneObject so{m_Owner, m_Scene};
            return so;
        }

        SceneObject GetOwner() const
        {
            if (m_Owner == entt::null)
                return {entt::to_entity(m_Scene->m_Registry, this), m_Scene};
            else
                return {m_Owner, m_Scene};
        }

        void MarkPositionDirty()
        {
            //mark for all children
            for (auto& child : GetOwner().GetChildren())
            {
                SceneObject so = {child, m_Scene};
                so.GetComponent<TransformComponent>().MarkPositionDirty();
            }
            m_IsWorldPositionDirty = true;
            m_IsTransformDirty = true;
        }

        void MarkRotationDirty()
        {
            //mark for all children
            for (auto& child : GetOwner().GetChildren())
            {
                SceneObject so = {child, m_Scene};
                so.GetComponent<TransformComponent>().MarkRotationDirty();
            }
            m_IsWorldRotationDirty = true;
            m_IsTransformDirty = true;
        }

        void MarkScaleDirty()
        {
            //mark for all children
            for (auto& child : GetOwner().GetChildren())
            {
                SceneObject so = {child, m_Scene};
                so.GetComponent<TransformComponent>().MarkScaleDirty();
            }
            m_IsWorldScaleDirty = true;
            m_IsTransformDirty = true;
        }

        Vec3 Position() const { return m_Position; }
        Vec3 Rotation() const { return m_Rotation; }
        Vec3 Scale() const { return m_Scale; }

        void SetPosition(const Vec3& position)
        {
            if (position == m_Position)
                return;
            m_Position = position;
            MarkPositionDirty();
        }

        void SetRotation(const Vec3& rotation)
        {
            if (rotation == m_Rotation)
                return;
            m_Rotation = rotation;
            MarkRotationDirty();
        }

        void SetScale(const Vec3& scale)
        {
            if (scale == m_Scale)
                return;
            m_Scale = scale;
            MarkScaleDirty();
        }

        Vec3 WorldPosition()
        {
            if (m_IsWorldPositionDirty)
            {
                Vec3 result = m_Position;
                SceneObject so = GetOwner();
                if (so.HasParent())
                {
                    so = so.GetParent();
                    if (so.HasComponent<TransformComponent>())
                    {
                        auto& tc = so.GetComponent<TransformComponent>();
                        result += so.GetComponent<TransformComponent>().WorldPosition();
                    }
                }
                m_WorldPosition = result;
            }

            m_IsWorldPositionDirty = false;
            return m_WorldPosition;
        }

        Vec3 WorldRotation()
        {
            if (m_IsWorldRotationDirty)
            {
                Vec3 result = m_Rotation;
                SceneObject so = GetOwner();
                if (so.HasParent())
                {
                    so = so.GetParent();
                    if (so.HasComponent<TransformComponent>())
                    {
                        auto& tc = so.GetComponent<TransformComponent>();
                        result += so.GetComponent<TransformComponent>().WorldRotation();
                    }
                }
                m_WorldRotation = result;
            }

            m_IsWorldRotationDirty = false;
            return m_WorldRotation;
        }

        Vec3 WorldScale()
        {
            if (m_IsWorldScaleDirty)
            {
                Vec3 result = m_Scale;
                SceneObject so = GetOwner();
                if (so.HasParent())
                {
                    so = so.GetParent();
                    if (so.HasComponent<TransformComponent>())
                    {
                        auto& tc = so.GetComponent<TransformComponent>();
                        result *= so.GetComponent<TransformComponent>().WorldScale();
                    }
                }
                m_WorldScale = result;
            }

            m_IsWorldScaleDirty = false;
            return m_WorldScale;
        }

    private:
        entt::entity m_Owner = {entt::null};
        Scene* m_Scene = nullptr;

        bool m_IsWorldPositionDirty = true;
        bool m_IsWorldRotationDirty = true;
        bool m_IsWorldScaleDirty = true;
        bool m_IsTransformDirty = true;

        Vec3 m_Position{0.0f, 0.0f, 0.0f};
        Vec3 m_Rotation{0.0f, 0.0f, 0.0f};
        Vec3 m_Scale{1.0f, 1.0f, 1.0f};

        Vec3 m_WorldPosition{0.0f, 0.0f, 0.0f};
        Vec3 m_WorldRotation{0.0f, 0.0f, 0.0f};
        Vec3 m_WorldScale{1.0f, 1.0f, 1.0f};

        Mat4 m_transform{1.0f};
    };
}

#include "MrgPch.h"

#include "TransformComponent.h"

#include "glm/ext/scalar_common.hpp"
#include "Mirage/ECS/SceneObject.h"
#include "Mirage/Math/Math.h"

namespace Mirage
{
    Mat4 TransformComponent::GetLocalTransform()
    {
        return MatTranslate(Mat4(1.0f), m_Position) *
            ToMat4(m_RotationQ) *
            MatScale(Mat4(1.0f), m_Scale);
    }

    Mat4 TransformComponent::GetTransform()
    {
        if (m_IsTransformDirty)
        {
            m_transform = MatTranslate(Mat4(1.0f), m_Position) *
                ToMat4(m_RotationQ) *
                MatScale(Mat4(1.0f), m_Scale);

            SceneObject so{GetOwner(), m_Scene};
            if (so.HasParent())
            {
                m_transform = so.GetParent().GetComponent<TransformComponent>().GetTransform() * m_transform;
            }

            m_IsTransformDirty = false;
        }

        return m_transform;
    }

    SceneObject TransformComponent::GetOwner()
    {
        if (m_Owner == entt::null)
            m_Owner = entt::to_entity<entt::entity, TransformComponent>(m_Scene->m_Registry, *this);

        SceneObject so{m_Owner, m_Scene};
        return so;
    }

    SceneObject TransformComponent::GetOwner() const
    {
        if (m_Owner == entt::null)
            return {entt::to_entity(m_Scene->m_Registry, this), m_Scene};
        else
            return {m_Owner, m_Scene};
    }

    void TransformComponent::MarkPositionDirty()
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

    void TransformComponent::MarkRotationDirty()
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

    void TransformComponent::MarkScaleDirty()
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

    Vec3 TransformComponent::WorldToLocalPosition(const Vec3& worldPosition)
    {
        return glm::inverse(GetTransform()) * Vec4(worldPosition, 1.0f);
    }

    void TransformComponent::SetPosition(const Vec3& position)
    {
        if (position == m_Position)
            return;
        m_Position = position;
        MarkPositionDirty();
    }

    void TransformComponent::SetWorldPosition(const Vec3& position)
    {
        if (position == WorldPosition())
            return;
        if (GetOwner().HasParent())
        {
            SceneObject so = {GetOwner().GetParent(), m_Scene};
            m_Position = so.GetComponent<TransformComponent>().WorldToLocalPosition(position);
        }
        else
        {
            m_Position = position;
        }
        MarkPositionDirty();
    }

    void TransformComponent::SetRotation(const Vec3& rotation)
    {
        if (rotation == m_Rotation)
            return;
        m_Rotation = rotation;
        SetRotation(Quat(Radians(rotation)));
    }

    void TransformComponent::SetRotation(const Quat& rotation)
    {
        if (rotation == m_RotationQ)
            return;
        m_RotationQ = rotation;
        MarkRotationDirty();
    }

    void TransformComponent::SetWorldRotation(const Vec3& rotation)
    {
        if (rotation == WorldRotation())
            return;
        Vec3 wr = WorldRotation();
        m_Rotation += (rotation - wr);
        SetWorldRotation(Quat(Radians(rotation)));
    }

    void TransformComponent::SetWorldRotation(const Quat& rotation)
    {
        if (rotation == WorldRotationQ())
            return;
        if (GetOwner().HasParent())
        {
            SceneObject so = {GetOwner().GetParent(), m_Scene};
            m_RotationQ = glm::inverse(so.GetComponent<TransformComponent>().WorldRotationQ()) * rotation;
        }
        else
        {
            m_RotationQ = rotation;
        }
        MarkRotationDirty();
    }

    void TransformComponent::SetScale(const Vec3& scale)
    {
        if (scale == m_Scale)
            return;
        m_Scale = scale;
        MarkScaleDirty();
    }

    void TransformComponent::SetWorldScale(const Vec3& scale)
    {
        if (scale == WorldScale())
            return;
        if (GetOwner().HasParent())
        {
            SceneObject so = {GetOwner().GetParent(), m_Scene};
            m_Scale = scale / so.GetComponent<TransformComponent>().WorldScale();
        }
        else
        {
            m_Scale = scale;
        }
        MarkScaleDirty();
    }

    void TransformComponent::Translate(const Vec3& translation)
    {
        m_Position += translation;
        MarkPositionDirty();
    }

    void TransformComponent::Rotate(const Vec3& rotation)
    {
        m_Rotation += rotation;
        Rotate(Quat(Radians(rotation)));
    }

    void TransformComponent::Rotate(const Quat& rotation)
    {
        if (GetOwner().HasParent())
        {
            SceneObject parent = GetOwner().GetParent();
            Quat parentRotation = parent.GetComponent<TransformComponent>().WorldRotationQ();
            m_RotationQ = glm::inverse(parentRotation) * rotation * parentRotation * m_RotationQ;
        }
        else
        {
            m_RotationQ = rotation * m_RotationQ;
        }
        MarkRotationDirty();
    }

    void TransformComponent::Scale(const Vec3& scale)
    {
        m_Scale *= scale;
        MarkScaleDirty();
    }


    Vec3 TransformComponent::WorldPosition()
    {
        if (m_IsWorldPositionDirty)
        {
            m_WorldPosition = GetTransform()[3];
            m_IsWorldPositionDirty = false;
        }

        return m_WorldPosition;
    }

    Vec3 TransformComponent::WorldRotation()
    {
        if(m_IsWorldRotationDirty)
        {
            m_WorldRotation = m_Rotation;
            if (GetOwner().HasParent())
            {
                SceneObject parent = GetOwner().GetParent();
                m_WorldRotation += parent.GetComponent<TransformComponent>().WorldRotation();
            }
            m_IsWorldRotationDirty = false;
        }

        return m_WorldRotation;
    }

    Quat TransformComponent::WorldRotationQ()
    {
        if (GetOwner().HasParent())
        {
            SceneObject parent = GetOwner().GetParent();
            return parent.GetComponent<TransformComponent>().WorldRotationQ() * m_RotationQ;
        }
        return m_RotationQ;
    }

    Vec3 TransformComponent::WorldScale()
    {
        if (m_IsWorldScaleDirty)
        {
            m_WorldScale = m_Scale;
            if (GetOwner().HasParent())
            {
                SceneObject so = GetOwner().GetParent();
                m_WorldScale *= so.GetComponent<TransformComponent>().WorldScale();
            }
            m_IsWorldScaleDirty = false;
        }

        return m_WorldScale;
    }
}

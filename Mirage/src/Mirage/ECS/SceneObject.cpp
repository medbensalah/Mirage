#include <MrgPch.h>
#include "SceneObject.h"

#include "Components/TagComponent.h"
#include "Components/TransformComponent.h"

namespace Mirage
{
    SceneObject::SceneObject(entt::entity entity, Scene* scene)
        : m_Entity(entity), m_Scene(scene)
    {
    }

    void SceneObject::Destroy()
    {
        m_Scene->m_Registry.destroy(m_Entity);
    }

}

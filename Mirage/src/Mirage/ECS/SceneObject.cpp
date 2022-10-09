#include <MrgPch.h>
#include "SceneObject.h"

namespace Mirage
{
    SceneObject::SceneObject(entt::entity entity, Scene* scene, const std::string& name)
        : m_name(name), m_Entity(entity), m_Scene(scene)
    {
    }
}

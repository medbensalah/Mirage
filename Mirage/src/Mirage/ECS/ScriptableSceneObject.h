#pragma once

#include "SceneObject.h"

namespace Mirage
{
    class ScriptableSceneObject
    {
    public:
        virtual ~ScriptableSceneObject() {}
        
        template <typename T>
        T& GetComponent()
        {
            return m_SceneObject.GetComponent<T>();
        }

    protected:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(float DeltaTime) {}
        
    private:
        SceneObject m_SceneObject;

        friend class Scene;
    };
}

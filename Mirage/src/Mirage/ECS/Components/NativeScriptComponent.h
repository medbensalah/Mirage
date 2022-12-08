#pragma once

namespace Mirage
{
	class ScriptableSceneObject;
	
    struct NativeScriptComponent
    {
        ScriptableSceneObject* Instance = nullptr;

        ScriptableSceneObject*(*InstantiateScript)();
       void (*DestroyScript)(NativeScriptComponent*);
                
        template<typename T>
        void Bind()
        {
            InstantiateScript = []() { return static_cast<ScriptableSceneObject*>(new T()); };
            DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
        }
    };
}

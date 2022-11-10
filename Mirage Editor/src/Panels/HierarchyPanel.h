#pragma once

#include "Mirage/Core/Core.h"

#include "Mirage/ECS/Scene.h"
#include "Mirage/ECS/SceneObject.h"


namespace Mirage
{
    class HierarchyPanel
    {
    public:
        HierarchyPanel() = default;
        HierarchyPanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);

        void OnImguiRender();

        SceneObject GetSelectedSO() const { return m_SelectionContext; }
        
    private:
            void DrawEntityNode(SceneObject so);
            void DrawComponents(SceneObject so);
    private:
        Ref<Scene> m_Context;

        SceneObject m_SelectionContext;
    };
    
}

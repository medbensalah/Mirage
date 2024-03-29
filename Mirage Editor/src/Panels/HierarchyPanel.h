﻿#pragma once

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
        void SetSelectedSO(SceneObject so, bool ignoreHierarchy = false);

        void OnImGuiRender();

        SceneObject GetSelectedSO() const { return m_SelectionContext; }
        
    private:
    		template<typename T>
			void DisplayAddComponentEntry(const std::string& entryName);
    	
            void DrawEntityNode(SceneObject so);
            void DrawComponents(SceneObject so);
    private:
        Ref<Scene> m_Context;

        SceneObject m_SelectionContext;
    };
}

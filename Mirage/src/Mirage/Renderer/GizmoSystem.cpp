#include "mrgpch.h"
#include "GizmoSystem.h"

#include <vector>

namespace Mirage
{
	namespace GizmoSystem
	{
		static std::vector<DrawSceneGizmoFn>& GetDrawers()
		{
			static std::vector<DrawSceneGizmoFn> s_Drawers;
			return s_Drawers;
		}

		bool RegisterDrawer(DrawSceneGizmoFn drawFn)
		{
			auto& drawers = GetDrawers();
			drawers.push_back(drawFn);
			return true;
		}

		void DrawSceneGizmos(Scene& scene, const EditorCamera& camera, int selectedEntityID)
		{
			for (auto drawFn : GetDrawers())
				drawFn(scene, camera, selectedEntityID);
		}
	}
}

#pragma once

namespace Mirage
{
	class Scene;
	class EditorCamera;

	namespace GizmoSystem
	{
		using DrawSceneGizmoFn = void(*)(Scene& scene, const EditorCamera& camera, int selectedEntityID);

		bool RegisterDrawer(DrawSceneGizmoFn drawFn);
		void DrawSceneGizmos(Scene& scene, const EditorCamera& camera, int selectedEntityID);
	}
}

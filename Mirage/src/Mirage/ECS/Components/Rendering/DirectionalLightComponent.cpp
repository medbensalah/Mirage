#include "mrgpch.h"
#include "DirectionalLightComponent.h"

#include "Mirage/ECS/Scene.h"
#include "Mirage/ECS/Components/Base/TransformComponent.h"
#include "Mirage/Renderer/GizmoSystem.h"
#include "Mirage/Renderer/Renderer2D.h"
#include "Mirage/Renderer/Texture.h"
#include "Mirage/Renderer/EditorCamera.h"
#include "Mirage/Definitions/EditorSettings.h"

namespace Mirage
{
	namespace
	{
		void DrawDirectionalLightGizmos(Scene& scene, const EditorCamera& camera, int selectedEntityID)
		{
			static Ref<Texture2D> icon = Texture2D::Create("Resources/Icons/Gizmos/DirectionalLight.png");
			auto lights = scene.GetSceneObjectsWith<TransformComponent, DirectionalLightComponent>();
			for (auto entity : lights)
			{
				auto [t, l] = lights.get<TransformComponent, DirectionalLightComponent>(entity);
				Vec3 p = t.WorldPosition();
				Vec4 lightColor = Vec4(l.Color, EditorSettings::GizmoIconOpacity);
				float size = (camera.IsOrthographic() ? 0.6f : glm::max(glm::distance(camera.GetPosition(), p) * 0.06f, 0.25f))
					* EditorSettings::GizmoIconSize;
				Mat4 transform = glm::translate(Mat4(1.0f), p) * ToMat4(camera.GetOrientation()) * glm::scale(Mat4(1.0f), Vec3(size));
				if (icon && icon->IsLoaded())
					Renderer2D::Draw::Quad(transform, lightColor, icon, Vec2(1.0f), Vec2(0.0f), (int)entity);

				Vec3 dir = glm::normalize(t.WorldRotationQ() * Vec3(0.0f, -1.0f, 0.0f));
				const Vec4 color = selectedEntityID == (int)entity ? Vec4(l.Color, 1.0f) : Vec4(l.Color, 0.9f);
				Vec3 start = p + dir * (size * 0.45f);
				Vec3 tip = start + dir * (size * 1.1f);
				Renderer2D::Draw::Line(start, tip, color, (int)entity);
				Vec3 side = glm::normalize(glm::cross(dir, Vec3(0.0f, 0.0f, 1.0f)));
				if (glm::length(side) < 0.001f)
					side = Vec3(1.0f, 0.0f, 0.0f);
				Renderer2D::Draw::Line(tip, tip - dir * (size * 0.25f) + side * (size * 0.2f), color, (int)entity);
				Renderer2D::Draw::Line(tip, tip - dir * (size * 0.25f) - side * (size * 0.2f), color, (int)entity);
			}
		}

	}

	void RegisterDirectionalLightGizmoDrawer()
	{
		static bool registered = false;
		if (!registered)
		{
			GizmoSystem::RegisterDrawer(DrawDirectionalLightGizmos);
			registered = true;
		}
	}
}

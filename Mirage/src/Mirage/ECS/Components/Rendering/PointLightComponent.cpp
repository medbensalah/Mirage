#include "mrgpch.h"
#include "PointLightComponent.h"

#include <cmath>

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
		void DrawPointLightGizmos(Scene& scene, const EditorCamera& camera, int selectedEntityID)
		{
			static Ref<Texture2D> icon = Texture2D::Create("Resources/Icons/Gizmos/PointLight.png");
			auto lights = scene.GetSceneObjectsWith<TransformComponent, PointLightComponent>();
			for (auto entity : lights)
			{
				auto [t, l] = lights.get<TransformComponent, PointLightComponent>(entity);
				Vec3 p = t.WorldPosition();
				Vec4 lightColor = Vec4(l.Color, EditorSettings::GizmoIconOpacity);
				float size = (camera.IsOrthographic() ? 0.6f : glm::max(glm::distance(camera.GetPosition(), p) * 0.06f, 0.25f))
					* EditorSettings::GizmoIconSize;
				Mat4 transform = glm::translate(Mat4(1.0f), p) * ToMat4(camera.GetOrientation()) * glm::scale(Mat4(1.0f), Vec3(size));
				if (icon && icon->IsLoaded())
					Renderer2D::Draw::Quad(transform, lightColor, icon, Vec2(1.0f), Vec2(0.0f), (int)entity);

				if ((int)entity == selectedEntityID)
				{
					const Vec4 color = Vec4(l.Color, 1.0f);
					float r = glm::max(l.Radius * 0.1f, 0.15f);
					const int seg = 20;
					for (int i = 0; i < seg; i++)
					{
						float a0 = (float)i / (float)seg * glm::two_pi<float>();
						float a1 = (float)(i + 1) / (float)seg * glm::two_pi<float>();
						Renderer2D::Draw::Line(p + Vec3(cos(a0) * r, sin(a0) * r, 0.0f), p + Vec3(cos(a1) * r, sin(a1) * r, 0.0f), color, (int)entity);
						Renderer2D::Draw::Line(p + Vec3(cos(a0) * r, 0.0f, sin(a0) * r), p + Vec3(cos(a1) * r, 0.0f, sin(a1) * r), color, (int)entity);
						Renderer2D::Draw::Line(p + Vec3(0.0f, cos(a0) * r, sin(a0) * r), p + Vec3(0.0f, cos(a1) * r, sin(a1) * r), color, (int)entity);
					}
				}
			}
		}

	}

	void RegisterPointLightGizmoDrawer()
	{
		static bool registered = false;
		if (!registered)
		{
			GizmoSystem::RegisterDrawer(DrawPointLightGizmos);
			registered = true;
		}
	}
}

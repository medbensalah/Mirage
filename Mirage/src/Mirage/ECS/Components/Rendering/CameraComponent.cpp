#include "mrgpch.h"
#include "CameraComponent.h"

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
		void DrawCameraGizmos(Scene& scene, const EditorCamera& editorCamera, int selectedEntityID)
		{
			(void)selectedEntityID;
			const Vec4 color = EditorSettings::DefaultGizmosColor;
			static Ref<Texture2D> icon = Texture2D::Create("Resources/Icons/Gizmos/Camera.png");
			auto cameras = scene.GetSceneObjectsWith<TransformComponent, CameraComponent>();
			for (auto entity : cameras)
			{
				auto [t, cameraComponent] = cameras.get<TransformComponent, CameraComponent>(entity);
				SceneCamera& camera = cameraComponent.Camera;
				const Vec3 pos = t.WorldPosition();
				const Quat rot = t.WorldRotationQ();
				const float aspect = glm::max(camera.GetAsectRatio(), 0.01f);
				const auto tp = [&](const Vec3& local) { return pos + rot * local; };
				float iconSize = (editorCamera.IsOrthographic() ? 0.6f : glm::max(glm::distance(editorCamera.GetPosition(), pos) * 0.06f, 0.25f))
					* EditorSettings::GizmoIconSize;
				Mat4 iconTransform = glm::translate(Mat4(1.0f), pos) * ToMat4(editorCamera.GetOrientation()) * glm::scale(Mat4(1.0f), Vec3(iconSize));
				if (icon && icon->IsLoaded())
					Renderer2D::Draw::Quad(iconTransform, Vec4(color.r, color.g, color.b, EditorSettings::GizmoIconOpacity), icon, Vec2(1.0f), Vec2(0.0f), (int)entity);

				if (camera.GetProjectionType() == Camera::ProjectionType::Orthographic)
				{
					const float previewDepth = glm::max(EditorSettings::CameraFrustumGizmoExtent, 0.1f);
					const float nearClip = glm::max(camera.GetOrthographicNearClip(), 0.0f);
					const float farClip = glm::max(glm::min(camera.GetOrthographicFarClip(), nearClip + previewDepth), nearClip + 0.001f);
					const float hh = camera.GetOrthographicSize() * 0.5f;
					const float hw = hh * aspect;
					Vec3 nc[4] = {tp({-hw, -hh, -nearClip}), tp({hw, -hh, -nearClip}), tp({hw, hh, -nearClip}), tp({-hw, hh, -nearClip})};
					Vec3 fc[4] = {tp({-hw, -hh, -farClip}), tp({hw, -hh, -farClip}), tp({hw, hh, -farClip}), tp({-hw, hh, -farClip})};
					for (int i = 0; i < 4; i++)
					{
						int n = (i + 1) % 4;
						Renderer2D::Draw::Line(nc[i], nc[n], color);
						Renderer2D::Draw::Line(fc[i], fc[n], color);
						Renderer2D::Draw::Line(nc[i], fc[i], color);
					}
				}
				else
				{
					const float previewDepth = glm::max(EditorSettings::CameraFrustumGizmoExtent, 0.1f);
					const float nearClip = glm::max(camera.GetPerspectiveNearClip(), 0.001f);
					const float farClip = glm::max(glm::min(camera.GetPerspectiveFarClip(), nearClip + previewDepth), nearClip + 0.001f);
					const float tanHalfFov = tanf(camera.GetPerspectiveVFOV() * 0.5f);
					const float nhh = tanHalfFov * nearClip;
					const float nhw = nhh * aspect;
					const float fhh = tanHalfFov * farClip;
					const float fhw = fhh * aspect;
					Vec3 nc[4] = {tp({-nhw, -nhh, -nearClip}), tp({nhw, -nhh, -nearClip}), tp({nhw, nhh, -nearClip}), tp({-nhw, nhh, -nearClip})};
					Vec3 fc[4] = {tp({-fhw, -fhh, -farClip}), tp({fhw, -fhh, -farClip}), tp({fhw, fhh, -farClip}), tp({-fhw, fhh, -farClip})};
					for (int i = 0; i < 4; i++)
					{
						int n = (i + 1) % 4;
						Renderer2D::Draw::Line(nc[i], nc[n], color);
						Renderer2D::Draw::Line(fc[i], fc[n], color);
						Renderer2D::Draw::Line(nc[i], fc[i], color);
						Renderer2D::Draw::Line(pos, fc[i], color);
					}
				}
			}
		}

	}

	void RegisterCameraGizmoDrawer()
	{
		static bool registered = false;
		if (!registered)
		{
			GizmoSystem::RegisterDrawer(DrawCameraGizmos);
			registered = true;
		}
	}
}

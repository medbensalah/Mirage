#include "EditorLayer.h"

#include <filesystem>
#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.inl>

#include "Mirage/Definitions/FileExtensions.h"
#include "Mirage/Definitions/DragnDropPayloads.h"
#include "Mirage/Definitions/Icons.h"
#include "glm/ext/scalar_common.hpp"
#include "glm/gtx/log_base.hpp"
#include "glm/gtx/transform2.hpp"
#include "glm/gtx/vector_angle.inl"
#include "Mirage/Definitions/Paths.h"
#include "Mirage/ECS/Components/Rendering/CameraComponent.h"
#include "Mirage/ImGui/Extensions/ButtonExtensions.h"
#include "Mirage/ImGui/Extensions/ToggleButton.h"

#include "Mirage/ECS/Components/Base/TransformComponent.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"

#include "Mirage/ECS/SceneSerializer.h"
#include "Mirage/ECS/Components/Physics/BoxCollider2DComponent.h"
#include "Mirage/ECS/Components/Physics/CircleCollider2DComponent.h"
#include "Mirage/Math/Math.h"
#include "Mirage/utils/PlatformUtils.h"

namespace Mirage
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	static ImGuiWindowClass s_ViewportWindowClass;

	void EditorLayer::OnAttach()
	{
		MRG_PROFILE_FUNCTION();
		s_ViewportWindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_CentralNode;

		m_texture = Texture2D::Create("assets/textures/CheckerBoard.png");

		FramebufferSpecs fbSpecs;
		fbSpecs.Attachments = {
			FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth
		};
		fbSpecs.Width = 1280;
		fbSpecs.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpecs);

		FramebufferSpecs previewFbSpecs;
		previewFbSpecs.Attachments = {
			FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth
		};
		previewFbSpecs.Width = m_PreviewSize.x;
		previewFbSpecs.Height = m_PreviewSize.y;
		m_PreviewFramebuffer = Framebuffer::Create(previewFbSpecs);

		m_ActiveScene = CreateRef<Scene>();

		auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(m_ActiveScene);
			serializer.SerializeText(sceneFilePath);
		}

		m_EditorCamera = EditorCamera(Camera::Orthographic);
		m_HierarchyPanel.SetContext(m_ActiveScene);
		m_SettingsPanel.LoadSettings();

		// ----------------------- initialize icons -----------------------
		m_PlayButtonIcon = Texture2D::Create(Icons::PlayButton);
		m_SimulateButtonIcon = Texture2D::Create(Icons::SimulateButton);
		m_StopButtonIcon = Texture2D::Create(Icons::StopButton);

		m_WorldSpaceIcon = Texture2D::Create(Icons::WorldSpaceIcon);
		m_LocalSpaceIcon = Texture2D::Create(Icons::LocalSpaceIcon);

		m_TranslationIcon = Texture2D::Create(Icons::TranslationIcon);
		m_RotationIcon = Texture2D::Create(Icons::RotationIcon);
		m_ScaleIcon = Texture2D::Create(Icons::ScaleIcon);
	}

	void EditorLayer::OnDetach()
	{
		MRG_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(float DeltaTime)
	{
		MRG_PROFILE_FUNCTION();
		// MRG_TRACE("Frame time : {0:.3f} ms   -   FPS : {1:.1f}", DeltaTime * 1000.0f, 1.0f / DeltaTime);
		// MRG_TRACE("Total elapsed time : {0:.3f} s", Application::GetSeconds());

		// --------------------------------- Resize ---------------------------------

		if (FramebufferSpecs spec = m_Framebuffer->GetSpecs();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			((uint32_t)spec.Width != (uint32_t)m_ViewportSize.x || (uint32_t)spec.Height != (uint32_t)m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		m_ShowPreview = m_ActiveScene->GetMainCameraSO();
		if (m_ShowPreview)
		{
			m_SceneCamera = m_ActiveScene->GetMainCameraSO().GetComponent<CameraComponent>().Camera;
			if (FramebufferSpecs pspec = m_PreviewFramebuffer->GetSpecs();
				m_PreviewSize.x > 0.0f && m_PreviewSize.y > 0.0f && // zero sized framebuffer is invalid
				((uint32_t)pspec.Width != (uint32_t)m_PreviewSize.x || (uint32_t)pspec.Height != (uint32_t)m_PreviewSize
					.y))
			{
				m_PreviewFramebuffer->Resize((uint32_t)m_PreviewSize.x, (uint32_t)m_PreviewSize.y);
				m_ActiveScene->OnViewportResize((uint32_t)m_PreviewSize.x, (uint32_t)m_PreviewSize.y);
				m_ActiveScene->GetMainCameraSO().GetComponent<CameraComponent>().Camera.SetViewportSize(
					(uint32_t)m_PreviewSize.x, (uint32_t)m_PreviewSize.y);
			}
		}
		// --------------------------------- Update ---------------------------------

		if (m_ViewportVisible)
		{
			m_EditorCamera.OnUpdate(DeltaTime);
		}

		// --------------------------------- Render ---------------------------------

		Renderer2D::ResetStats();


		if (m_ViewportVisible)
		{
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor(m_EditorCamera.GetClearColor());
			RenderCommand::Clear();
			m_Framebuffer->ClearAttachment(1, -1);
			m_ActiveScene->OnUpdateEditor(DeltaTime, m_EditorCamera);
			OnOverlayRender();
			m_Framebuffer->Unbind();
		}

		if (m_ShowPreview && m_GamePreviewVisible)
		{
			m_PreviewFramebuffer->Bind();
			RenderCommand::SetClearColor(m_SceneCamera.GetClearColor());
			RenderCommand::Clear();
			m_PreviewFramebuffer->ClearAttachment(1, -1);
			switch (m_SceneState)
			{
			case SceneState::Edit:
				m_ActiveScene->RenderRuntime();
				break;
			case SceneState::Play:
				m_ActiveScene->OnUpdateRuntime(DeltaTime);
				break;
			case SceneState::Simulate:
				m_ActiveScene->OnUpdateSimulation(DeltaTime, m_EditorCamera);
				break;
			}
			m_PreviewFramebuffer->Unbind();
		}
	}

	static bool showDemo = false;

	void EditorLayer::CreateDockspace()
	{
		MRG_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {300.0f, 25.0f});
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		ImGui::PopStyleVar();

		CreateMenuBar();

		ImGui::End();
	}

	void EditorLayer::CreateMenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl + N"))
					NewScene();


				if (ImGui::MenuItem("Open...", "Ctrl + O"))
					OpenScene();

				// TODO: Save

				if (ImGui::MenuItem("Save as...", "Ctrl + shift + S"))
					SaveAs();


				ImGui::Separator();
				if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Settings"))
			{
				if (ImGui::MenuItem("Settings"))
					m_ShowSettings = true;

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Stats"))
					m_ShowStats = true;

				if (ImGui::MenuItem("Graph Editor"))
					m_ShowGraphEditor = true;

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void EditorLayer::CreateViewport()
	{
		MRG_PROFILE_FUNCTION();
		ImGui::SetNextWindowClass(&s_ViewportWindowClass);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

		if (ImGui::Begin("Viewport"))
		{
			auto viewportMinRegion = ImGui::GetCursorPos();
			// auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			m_ViewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
			m_ViewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};


			// ------------------------- Viewport Events -------------------------
			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			bool IsAnyImGuiActive = ImGui::IsAnyItemActive();

			if (m_SceneState == SceneState::Edit)
			{
				Application::Get().GetImGuiLayer()->AllowKbEvents(!IsAnyImGuiActive);
				Application::Get().GetImGuiLayer()->AllowMouseEvents(m_ViewportHovered);
			}
			else
			{
				Application::Get().GetImGuiLayer()->AllowKbEvents(
					m_ViewportFocused && m_ViewportHovered && !IsAnyImGuiActive);
				Application::Get().GetImGuiLayer()->AllowMouseEvents(m_ViewportFocused || m_ViewportHovered);
			}

			// Application::Get().GetImGuiLayer()->AllowKbEvents(
			// 	(m_ViewportFocused || m_ViewportHovered) && !IsAnyImGuiActive);
			// Application::Get().GetImGuiLayer()->AllowMouseEvents(m_ViewportFocused || m_ViewportHovered);

			// ------------------------- Scene Viewport -------------------------
			ImVec2 ViewportSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = {ViewportSize.x, ViewportSize.y};
			uint32_t texId = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)texId, ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Payloads::scene.c_str()))
				{
					char path[512] = {};
					memcpy(path, payload->Data, payload->DataSize);
					OpenScene(path);
				}
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Payloads::texture.c_str()))
				{
					char path[512] = {};
					memcpy(path, payload->Data, payload->DataSize);

					Vec2 mousePos = GetMouseViewportSpace();
					int pixelData = GetIDat(mousePos);
					Ref<Texture2D> tex = Texture2D::Create(path);
					if (tex->IsLoaded())
					{
						if (pixelData == -1)
						{
							SceneObject so = m_ActiveScene->CreateSceneObject();

							// get world space mouse position
							Vec3 mousePosWS = GetMouseWorldSpace();
							so.GetComponent<TransformComponent>().SetPosition({mousePosWS.x, mousePosWS.y, 0.0f});

							so.AddComponent<SpriteRendererComponent>(Vec4{1, 1, 1, 1}, tex);
							m_HierarchyPanel.SetSelectedSO(so);
						}
						else
						{
							SceneObject so = m_ActiveScene->GetSceneObject((entt::entity)pixelData);
							if (so.HasComponent<SpriteRendererComponent>())
							{
								so.GetComponent<SpriteRendererComponent>().Texture = tex;
							}
							m_HierarchyPanel.SetSelectedSO(so, true);
						}
					}
				}
				ImGui::EndDragDropTarget();
			}

			// ----------------------------- Gizmos ------------------------------

			ImGuizmo::SetOrthographic(m_EditorCamera.IsOrthographic());
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y,
			                  m_ViewportBounds[1].x - m_ViewportBounds[0].x,
			                  m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			ImGuizmo::AllowAxisFlip(false);

			if (m_ShowGrid && m_GridAlpha)
			{
				MRG_PROFILE_SCOPE("Render grid");
				Mat4 matrix = glm::rotate(Mat4(1.0f), glm::radians(-90.0f), Vec3(1, 0, 0));

				ImGuizmo::DrawGrid(glm::value_ptr(m_EditorCamera.GetView()),
				                   glm::value_ptr(m_EditorCamera.GetProjection()),
				                   glm::value_ptr(matrix),
				                   m_GridSize, m_GridAlpha);
			}


			SceneObject selectedSO = m_HierarchyPanel.GetSelectedSO();

			if (selectedSO)
			{
				// Runitme Camera
				/*
				auto runtimeCamera = m_ActiveScene->GetMainCameraSO();
				const auto& camera = runtimeCamera.GetComponent<CameraComponent>().Camera;
				const Mat4& cameraProjection = camera.GetProjection();
				const Mat4& cameraView = Inverse(runtimeCamera.GetComponent<TransformComponent>().GetTransform());
				*/
				// Editor Camera
				const Mat4 cameraProjection = m_EditorCamera.GetProjection();
				const Mat4& cameraView = m_EditorCamera.GetView();

				auto& tc = selectedSO.GetComponent<TransformComponent>();

				Mat4 transform = tc.GetTransform();
				Mat4 delta;

				// Snapping

				float snapValues[3];

				bool snap = false;
				if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE)
				{
					snap = m_TranslationSnap;
					snapValues[0] = snapValues[1] = snapValues[2] = m_TranslationSnapValue;
				}
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				{
					snap = m_RotationSnap;
					snapValues[0] = snapValues[1] = snapValues[2] = m_RotationSnapValue;
				}
				if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
				{
					snap = m_ScaleSnap;
					snapValues[0] = snapValues[1] = snapValues[2] = m_ScaleSnapValue;
				}

				snap |= Input::IsKeyPressed(Key::LeftControl);

				bool GizmoAction;

				if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
				{
					GizmoAction = ImGuizmo::Manipulate(glm::value_ptr(cameraView),
					                                   glm::value_ptr(cameraProjection),
					                                   ImGuizmo::OPERATION::SCALE, ImGuizmo::LOCAL,
					                                   glm::value_ptr(transform), glm::value_ptr(delta),
					                                   snap ? snapValues : nullptr);
				}
				else
				{
					GizmoAction = ImGuizmo::Manipulate(glm::value_ptr(cameraView),
					                                   glm::value_ptr(cameraProjection),
					                                   m_GizmoType, m_GizmoMode,
					                                   glm::value_ptr(transform), glm::value_ptr(delta),
					                                   snap ? snapValues : nullptr);
				}
				if (GizmoAction)
				{
					Vec3 position, rotation, scale;
					Math::DecomposeTransform(delta, position, rotation, scale);
					rotation = Degrees(rotation);

					switch (m_GizmoType)
					{
					case ImGuizmo::OPERATION::TRANSLATE:
						tc.SetWorldPosition(tc.WorldPosition() + position);
						break;
					case ImGuizmo::OPERATION::ROTATE:
						tc.Rotate(rotation);
						break;
					case ImGuizmo::OPERATION::SCALE:
						Math::DecomposeTransform(transform, position, rotation, scale);
						tc.SetWorldScale(scale);
						break;
					}
				}
			}

			// ----------------------------- Toolbar -----------------------------
			ImGui::SetItemAllowOverlap();
			ImVec2 curPos(ImGui::GetWindowContentRegionMin());
			ImGui::SetCursorPos({curPos.x + 7.0f, curPos.y + 3.0f});
			ImGui::BeginGroup();
			CreateToolBar();
			ImGui::EndGroup();
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::CreateGamePreview()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
		if (ImGui::Begin("Game preview"))
		{
			auto& style = ImGui::GetStyle();
			float btnSizeX = m_PlayButtonIconSize + style.FramePadding.x * 2.0f;
			float btnSizeY = m_PlayButtonIconSize + style.FramePadding.y * 2.0f;
			float btnPosX = (ImGui::GetContentRegionAvail().x - btnSizeX * 2.0f) / 2.0f;
			float btnPosY = ImGui::GetCursorPosY() + 2.0f;

			// Play button
			ImGui::SetCursorPos({btnPosX, btnPosY});
			bool playStopBtnPressed = ImGui::GradientButton("##PlayStopButton", ImVec2{btnSizeX, btnSizeY});
			bool playStopBtnHovered = ImGui::IsItemHovered();
			ImGui::SetCursorPos({btnPosX + style.FramePadding.x, btnPosY + style.FramePadding.y});
			if (m_SceneState == SceneState::Edit)
			{
				ImGui::Image((ImTextureID)m_PlayButtonIcon->GetRendererID(),
				             ImVec2{m_PlayButtonIconSize, m_PlayButtonIconSize},
				             {0, 1}, {1, 0}, {0.03f, 0.7f, 0, 1}, {1, 1, 1, 0.0f});
			}
			else if (m_SceneState == SceneState::Play)
			{
				ImGui::Image((ImTextureID)m_StopButtonIcon->GetRendererID(),
				             ImVec2{m_PlayButtonIconSize, m_PlayButtonIconSize},
				             {0, 1}, {1, 0}, {0.7f, 0.03f, 0, 1}, {1, 1, 1, 0.0f});
			}
			else
			{
				ImGui::Image((ImTextureID)m_PlayButtonIcon->GetRendererID(),
				             ImVec2{m_PlayButtonIconSize, m_PlayButtonIconSize},
				             {0, 1}, {1, 0}, {0.7f, 0.7f, 0.7, 1}, {1, 1, 1, 0.0f});
			}

			// Simulate button
			ImGui::SetCursorPos({btnPosX + btnSizeX, btnPosY});
			bool simulateStopBtnPressed = ImGui::GradientButton("##SimulateButton", ImVec2{btnSizeX, btnSizeY});
			bool simulateStopBtnHovered = ImGui::IsItemHovered();
			ImGui::SetCursorPos({btnPosX + btnSizeX + style.FramePadding.x, btnPosY + style.FramePadding.y});
			if (m_SceneState == SceneState::Edit)
			{
				ImGui::Image((ImTextureID)m_SimulateButtonIcon->GetRendererID(),
				             ImVec2{m_PlayButtonIconSize, m_PlayButtonIconSize},
				             {0, 1}, {1, 0}, {0.03f, 0.7f, 0, 1}, {1, 1, 1, 0.0f});
			}
			else if (m_SceneState == SceneState::Simulate)
			{
				ImGui::Image((ImTextureID)m_StopButtonIcon->GetRendererID(),
				             ImVec2{m_PlayButtonIconSize, m_PlayButtonIconSize},
				             {0, 1}, {1, 0}, {0.7f, 0.03f, 0, 1}, {1, 1, 1, 0.0f});
			}
			else
			{
				ImGui::Image((ImTextureID)m_SimulateButtonIcon->GetRendererID(),
				             ImVec2{m_PlayButtonIconSize, m_PlayButtonIconSize},
				             {0, 1}, {1, 0}, {0.7f, 0.7f, 0.7, 1}, {1, 1, 1, 0.0f});
			}


			if (playStopBtnHovered)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{8, 2});
				if (m_SceneState == SceneState::Edit)
				{
					ImGui::SetTooltip("Play");
				}
				else if (m_SceneState == SceneState::Play)
				{
					ImGui::SetTooltip("Stop");
				}
				ImGui::PopStyleVar();
			}
			if (playStopBtnPressed)
			{
				if (m_SceneState == SceneState::Edit)
				{
					OnScenePlay();
				}
				else if (m_SceneState == SceneState::Play)
				{
					OnSceneStop();
				}
			}

			if (simulateStopBtnHovered)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{8, 2});
				if (m_SceneState == SceneState::Edit)
				{
					ImGui::SetTooltip("Simulate");
				}
				else if (m_SceneState == SceneState::Simulate)
				{
					ImGui::SetTooltip("Stop");
				}
				ImGui::PopStyleVar();
			}
			if (simulateStopBtnPressed)
			{
				if (m_SceneState == SceneState::Edit)
				{
					OnSceneSimulate();
				}
				else if (m_SceneState == SceneState::Simulate)
				{
					OnSceneStop();
				}
			}

			ImGui::SetCursorPosY(btnPosY + btnSizeY + style.FramePadding.y);

			uint32_t previewTexId = m_PreviewFramebuffer->GetColorAttachmentRendererID();
			float sizeY = ImGui::GetContentRegionAvail().y;
			float aspectRatio = 16.0f / 9;
			float sizeX = sizeY * aspectRatio;
			m_PreviewSize = {sizeX, sizeY};
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - sizeX) / 2.0f);
			ImGui::Image((void*)previewTexId, ImVec2{sizeX, sizeY}, ImVec2{0, 1}, ImVec2{1, 0});
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::CreateToolBar()
	{
		MRG_PROFILE_FUNCTION();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{3, 3});
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.000f, 0.500f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.000f, 0.000f, 0.000f, 0.500f));
		// -------------------------- Space --------------------------
#pragma region space
		const ImGuiStyle& style = ImGui::GetStyle();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Space : ");
		ImGui::SameLine();

		bool IsLocalSpace = m_GizmoMode == ImGuizmo::LOCAL;
		bool change;
		if (!IsLocalSpace)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
		}

		change = ImGui::ImageButton((ImTextureID)m_WorldSpaceIcon->GetRendererID(),
		                            ImVec2{m_IconSizeS, m_IconSizeS},
		                            {0, 1}, {1, 0}, -1);
		if (!IsLocalSpace)
		{
			ImGui::PopStyleColor(3);
		}

		ImGui::SameLine();

		if (IsLocalSpace)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
		}
		change |= ImGui::ImageButton((ImTextureID)m_LocalSpaceIcon->GetRendererID(),
		                             ImVec2{m_IconSizeS, m_IconSizeS},
		                             {0, 1}, {1, 0}, -1);
		if (IsLocalSpace)
		{
			ImGui::PopStyleColor(3);
		}
		if (change)
		{
			m_GizmoMode = m_GizmoMode == ImGuizmo::LOCAL ? ImGuizmo::WORLD : ImGuizmo::LOCAL;
		}
#pragma endregion
		// --------------------------- Mode --------------------------
#pragma region Mode
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
		ImGui::Text("Mode : ");
		ImGui::SameLine();

		bool isTranslating = m_GizmoType == ImGuizmo::OPERATION::TRANSLATE;
		bool isRotating = m_GizmoType == ImGuizmo::OPERATION::ROTATE;
		bool isScaling = m_GizmoType == ImGuizmo::OPERATION::SCALE;

		// ------------------- Translate -------------------
		if (isTranslating)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
		}
		if (ImGui::ImageButton((ImTextureID)m_TranslationIcon->GetRendererID(), ImVec2{m_IconSizeS, m_IconSizeS},
		                       {0, 1}, {1, 0}, -1))
		{
			if (isTranslating)
				m_GizmoType = (ImGuizmo::OPERATION)-1;
			else
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}
		if (isTranslating)
			ImGui::PopStyleColor(3);

		ImGui::SameLine();

		// ------------------- Rotate -------------------
		if (isRotating)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
		}
		if (ImGui::ImageButton((ImTextureID)m_RotationIcon->GetRendererID(), ImVec2{m_IconSizeS, m_IconSizeS},
		                       {0, 1}, {1, 0}, -1))
		{
			if (isRotating)
				m_GizmoType = (ImGuizmo::OPERATION)-1;
			else
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
		}
		if (isRotating)
			ImGui::PopStyleColor(3);


		ImGui::SameLine();

		// ------------------- Scale -------------------
		if (isScaling)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
		}
		if (ImGui::ImageButton((ImTextureID)m_ScaleIcon->GetRendererID(), ImVec2{m_IconSizeS, m_IconSizeS},
		                       {0, 1}, {1, 0}, -1))
		{
			if (isScaling)
				m_GizmoType = (ImGuizmo::OPERATION)-1;
			else
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
		}
		if (isScaling)
			ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();
#pragma endregion
		// --------------------------- Grid --------------------------
#pragma region Grid
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
		ImVec2 gridPopUpPos = ImGui::GetCursorPos();
		gridPopUpPos.y += 25.0f;
		if (ImGui::GradientButton("Grid", ImVec2{70.0f, 25.0f}))
		{
			ImGui::OpenPopup("Grid");
		}

		ImGui::SetNextWindowSize(ImVec2{215, 85});
		ImGui::SetNextWindowPos(ImVec2{
			ImGui::GetWindowPos().x + gridPopUpPos.x, ImGui::GetWindowPos().y + gridPopUpPos.y
		});
		if (ImGui::BeginPopup("Grid"))
		{
			ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));

			ImGui::AlignTextToFramePadding();
			ImGui::SetCursorPos(ImVec2{ImGui::GetCursorPosX() + 10, ImGui::GetCursorPosY() + 3});
			ImGui::Text("Show grid :");
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40);
			float curPos = ImGui::GetCursorPosX();
			ImGui::Checkbox("##ShowGrid", &m_ShowGrid);

			ImGui::AlignTextToFramePadding();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::Text("Transparency : ");
			ImGui::SameLine();
			ImGui::SetCursorPosX(curPos);
			ImGui::PushItemWidth(75);
			ImGui::DragFloat("##GridTransparency", &m_GridAlpha, 0.01f, 0.0f, 1.0f, "%.5g");
			ImGui::PopItemWidth();

			ImGui::AlignTextToFramePadding();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::Text("Grid size : ");
			ImGui::SameLine();
			ImGui::SetCursorPosX(curPos);
			ImGui::PushItemWidth(75);
			ImGui::DragFloat("##GridSize", &m_GridSize, 1.0f, 10.0f, 10000.0f, "%.5g");
			ImGui::PopItemWidth();

			ImGui::PopStyleColor();

			ImGui::EndPopup();
		}
#pragma endregion
		// ------------------------- Snapping ------------------------
#pragma region Snap
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
		ImVec2 snapPopUpPos = ImGui::GetCursorPos();
		snapPopUpPos.y += 25.0f;
		if (ImGui::GradientButton("Snapping", ImVec2{100.0f, 25.0f}))
		{
			ImGui::OpenPopup("Snapping");
		}

		ImGui::SetNextWindowSize(ImVec2{215, 85});
		ImGui::SetNextWindowPos(ImVec2{
			ImGui::GetWindowPos().x + snapPopUpPos.x, ImGui::GetWindowPos().y + snapPopUpPos.y
		});
		if (ImGui::BeginPopup("Snapping"))
		{
			ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));

			ImGui::AlignTextToFramePadding();
			ImGui::SetCursorPos(ImVec2{ImGui::GetCursorPosX() + 10, ImGui::GetCursorPosY() + 3});
			ImGui::Text("Translation :");
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 27);
			float curPos = ImGui::GetCursorPosX();
			ImGui::Checkbox("##TranslationSnap", &m_TranslationSnap);
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::GetCursorPosX();
			ImGui::DragFloat("##TranslationSnapping", &m_TranslationSnapValue, 0.1f, 0.0f, 1000.0f, "%.5g");
			ImGui::PopItemWidth();

			ImGui::AlignTextToFramePadding();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::Text("Rotation : ");
			ImGui::SameLine();
			ImGui::SetCursorPosX(curPos);
			ImGui::Checkbox("##RotationSnap", &m_RotationSnap);
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::DragFloat("##RotationSnapping", &m_RotationSnapValue, 1.0f, 0.0f, 180.0f, "%.5g");
			ImGui::PopItemWidth();

			ImGui::AlignTextToFramePadding();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::Text("Scale : ");
			ImGui::SameLine();
			ImGui::SetCursorPosX(curPos);
			ImGui::Checkbox("##ScaleSnap", &m_ScaleSnap);
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::DragFloat("##ScaleSnapping", &m_ScaleSnapValue, 0.01f, 0.0f, 1.0f, "%.5g");
			ImGui::PopItemWidth();

			ImGui::PopStyleColor();

			ImGui::EndPopup();
		}
#pragma endregion
		// -------------------------- Camera -------------------------
#pragma region Camera
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 110);
		ImVec2 cameraPopUpPos = ImGui::GetCursorPos();
		cameraPopUpPos.x -= 100.0f;
		cameraPopUpPos.y += 25.0f;
		if (ImGui::GradientButton("Camera", ImVec2{100.0f, 25.0f}))
		{
			ImGui::OpenPopup("Camera");
		}

		ImGui::SetNextWindowSize(ImVec2{200, 0});
		ImGui::SetNextWindowPos(ImVec2{
			ImGui::GetWindowPos().x + cameraPopUpPos.x, ImGui::GetWindowPos().y + cameraPopUpPos.y
		});

		if (ImGui::BeginPopup("Camera"))
		{
			ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
			bool is3D = !m_EditorCamera.IsOrthographic();

			ImGui::AlignTextToFramePadding();
			ImGui::SetCursorPos(ImVec2{ImGui::GetCursorPosX() + 10, ImGui::GetCursorPosY() + 3});
			ImGui::Text("Mode: ");
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 35);
			float curPos = ImGui::GetCursorPosX();
			ImGui::Text("2D");
			ImGui::SameLine();
			ImGui::PushItemWidth(110);
			if (ImGui::ToggleButton("##CameraMode", &is3D))
			{
				// is 3D after click
				m_EditorCamera.SetProjectionType(is3D
					                                 ? Camera::ProjectionType::Perspective
					                                 : Camera::ProjectionType::Orthographic);
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Text("3D");

			ImGui::Separator();
			ImGui::Spacing();

			ImGui::AlignTextToFramePadding();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::Text("Speed: ");
			ImGui::SameLine();
			ImGui::SetCursorPosX(curPos - 5);
			ImGui::PushItemWidth(100);
			ImGui::SliderFloat("##EditorCameraMoveSpeed", &m_EditorCamera.m_MoveSpeed, 0.01f, 10.0f, "%.5g");
			ImGui::PopItemWidth();

			if (!is3D)
			{
				ImGui::AlignTextToFramePadding();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				ImGui::Text("Size: ");
				ImGui::SameLine();
				ImGui::SetCursorPosX(curPos - 5);
				ImGui::PushItemWidth(100);
				if (ImGui::SliderFloat("##EditorCameraSize", &m_EditorCamera.m_OrthographicData.m_OrthographicSize,
				                       1.0f, 50.0f, "%.5g"))
				{
					m_EditorCamera.UpdateProjection();
				}
				ImGui::PopItemWidth();

				ImGui::Separator();
				ImGui::Spacing();

				ImGui::AlignTextToFramePadding();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				ImGui::Text("Near plane: ");
				ImGui::SameLine();
				ImGui::SetCursorPosX(curPos - 5);
				ImGui::PushItemWidth(100);
				if (ImGui::DragFloat("##EditorCameraNearClippingPlane",
				                     &m_EditorCamera.m_OrthographicData.m_OrthographicNear, 0.01f, 0.01, 0.0f, "%.5g"))
				{
					m_EditorCamera.UpdateProjection();
				}
				ImGui::PopItemWidth();

				ImGui::AlignTextToFramePadding();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				ImGui::Text("Far plane: ");
				ImGui::SameLine();
				ImGui::SetCursorPosX(curPos - 5);
				ImGui::PushItemWidth(100);
				if (ImGui::DragFloat("##EditorCameraFarClippingPlane",
				                     &m_EditorCamera.m_OrthographicData.m_OrthographicFar, 10.0f, 0.01f, 0.0f, "%.5g"))
				{
					m_EditorCamera.UpdateProjection();
				}
			}
			else
			{
				ImGui::AlignTextToFramePadding();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				ImGui::Text("FOV: ");
				ImGui::SameLine();
				ImGui::SetCursorPosX(curPos - 5);
				ImGui::PushItemWidth(100);
				if (ImGui::SliderFloat("##EditorCameraFOV", &m_EditorCamera.m_PerspectiveData.m_FOV, 10.0f, 120.0f,
				                       "%.5g"))
				{
					m_EditorCamera.UpdateProjection();
				}
				ImGui::PopItemWidth();

				ImGui::Separator();
				ImGui::Spacing();

				ImGui::AlignTextToFramePadding();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				ImGui::Text("Near plane: ");
				ImGui::SameLine();
				ImGui::SetCursorPosX(curPos + 5);
				ImGui::PushItemWidth(100);
				if (ImGui::DragFloat("##EditorCameraNearClippingPlane", &m_EditorCamera.m_PerspectiveData.m_NearClip,
				                     0.01f, 0.01, 0.0f, "%.5g"))
				{
					m_EditorCamera.UpdateProjection();
				}
				ImGui::PopItemWidth();

				ImGui::AlignTextToFramePadding();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				ImGui::Text("Far plane: ");
				ImGui::SameLine();
				ImGui::SetCursorPosX(curPos - 5);
				ImGui::PushItemWidth(100);
				if (ImGui::DragFloat("##EditorCameraFarClippingPlane", &m_EditorCamera.m_PerspectiveData.m_FarClip,
				                     0.1f, 0.01f, 0.0f, "%.5g"))
				{
					m_EditorCamera.UpdateProjection();
				}
				ImGui::PopItemWidth();
			}

			ImGui::Spacing();

			ImGui::Separator();
			ImGui::Spacing();

			ImGui::AlignTextToFramePadding();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::Text("Physics Overlay: ");
			ImGui::SameLine();
			ImGui::SetCursorPosX(curPos + 25);
			ImGui::PushItemWidth(-1);
			ImGui::Checkbox("##OverlayPhysicsColliders", &m_ShowPhysicsColliders);
			ImGui::PopItemWidth();


			ImGui::Spacing();

			ImGui::PopStyleColor();

			ImGui::EndPopup();
		}

#pragma endregion

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar();
	}

	void EditorLayer::OnImGuiRender()
	{
		MRG_PROFILE_FUNCTION();

		CreateDockspace();

		// ----------------------------- Main -----------------------------
		CreateGamePreview();
		CreateViewport();
		m_HierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		// ----------------------------- Extra -----------------------------
		// TODO: destroy and create
		if (m_ShowSettings)
		{
			m_SettingsPanel.OnImGuiRender(&m_ShowSettings);
			if (!m_ShowSettings)
			{
				m_SettingsPanel.SaveSettings();
			}
		}
		if (m_ShowGraphEditor)
		{
			m_GraphEditor.OnImGuiRender(&m_ShowGraphEditor);
		}


		if (m_ContentBrowserPanel.m_IsSceneRequested)
		{
			MRG_CORE_INFO("changing scene");
			OpenScene(m_ContentBrowserPanel.m_RequestedScenePath);
			m_ContentBrowserPanel.m_IsSceneRequested = false;
		}

		if (m_ShowStats)
		{
			ImGui::Begin("Stats", &m_ShowStats);

			float deltaTime = Application::Get().GetDeltaTime();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Indent();
			ImGui::Text("Draw Calls: %d", Renderer2D::GetStats().DrawCalls);
			ImGui::Text("Quads: %d", Renderer2D::GetStats().QuadCount);
			ImGui::Text("Circles: %d", Renderer2D::GetStats().CircleCount);
			ImGui::Text("Vertices: %d", Renderer2D::GetStats().GetVertexCount());
			ImGui::Text("Indices: %d", Renderer2D::GetStats().GetIndexCount());
			ImGui::Unindent();
			ImGui::Spacing();
			ImGui::TextDisabled("FPS : %3.1f  (%f ms)", 1.0f / deltaTime, deltaTime * 1000.0f);
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			DrawSplitUIItem("Show demo", [&]()-> bool
			{
				return ImGui::ToggleButton("##ShowDemo", &showDemo);
			});
			ImGui::End();

			if (showDemo)
			{
				ImGui::ShowDemoWindow(&showDemo);
			}
		}
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);

		// Shortcuts
		dispatcher.Dispatch<KeyPressedEvent>(MRG_BIND_EVENT_FN(EditorLayer::OnShortcutKeyPressed));

		// MousePicking
		dispatcher.Dispatch<MouseButtonPressedEvent>(MRG_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnShortcutKeyPressed(KeyPressedEvent e)
	{
		if (e.IsRepeat() > 0)
			return false;

		bool ctrl = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		bool rightMB = Input::IsMouseButtonPressed(Mouse::ButtonRight);

		if (ctrl)
		{
			// --------------------------------- Menu ---------------------------------
			switch (e.GetKeyCode())
			{
			case Key::N:
				NewScene();
				return true;
				break;
			case Key::O:
				OpenScene();
				return true;
				break;
			case Key::S:
				if (shift)
				{
					SaveAs();
					return true;
				}
				Save();
				break;
			// ------------------------------- Commands -------------------------------
			case Key::D:
				DuplicateSelected();
				return true;
			default:
				break;
			}
		}
		// --------------------------------- Gizmos ---------------------------------
		else if (rightMB)
		{
			return false;
		}
		else
		{
			switch (e.GetKeyCode())
			{
			case Key::Q:
				m_GizmoType = (ImGuizmo::OPERATION)-1;
				return true;
				break;
			case Key::W:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				return true;
				break;
			case Key::E:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				return true;
				break;
			case Key::R:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				return true;
				break;
			case Key::M:
				m_GizmoMode = m_GizmoMode == ImGuizmo::MODE::LOCAL ? ImGuizmo::MODE::WORLD : ImGuizmo::MODE::LOCAL;
				return true;
				break;
			default:
				break;
			}
		}
		return false;
	}

	bool EditorLayer::CanPick()
	{
		if (m_HierarchyPanel.GetSelectedSO())
		{
			return !ImGuizmo::IsOver() && m_ViewportFocused && m_ViewportHovered && !ImGui::IsAnyItemHovered();
		}
		return m_ViewportFocused && m_ViewportHovered && !ImGui::IsAnyItemHovered();
	}

	bool EditorLayer::IsInViewportSpace(glm::fvec2 pos)
	{
		auto [x, y] = ImGui::GetMousePos();
		x -= m_ViewportBounds[0].x;
		y -= m_ViewportBounds[0].y;
		Vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		return (pos.x >= 0 && pos.x < viewportSize.x && pos.y >= 0 && pos.y < viewportSize.y);
	}

	VecI2 EditorLayer::GetMouseViewportSpace()
	{
		auto [x, y] = ImGui::GetMousePos();
		x -= m_ViewportBounds[0].x;
		y -= m_ViewportBounds[0].y;
		Vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		return {(int)x, (int)(viewportSize.y - y)};
	}

	Vec3 EditorLayer::GetMouseWorldSpace()
	{
		auto [x, y] = ImGui::GetMousePos();
		x -= m_ViewportBounds[0].x;
		y -= m_ViewportBounds[0].y;
		Vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		// Normalized Device Coordinates
		float nx = (2.0f * x) / viewportSize.x - 1.0f;
		float ny = 1.0f - (2.0f * y) / viewportSize.y;
		float nz = 1.0f;
		Vec3 ray_nds = {nx, ny, nz};

		// Homogeneous Clip Coordinates
		Vec4 ray_clip = {ray_nds.x, ray_nds.y, -1.0f, 1.0f};

		// Eye (Camera) Coordinates
		auto invProj = glm::inverse(m_EditorCamera.GetProjection());
		Vec4 ray_eye = invProj * ray_clip;
		ray_eye = {ray_eye.x, ray_eye.y, -1.0f, 0.0f};

		// World Coordinates
		auto invView = glm::inverse(m_EditorCamera.GetView());
		Vec3 posOffset = m_EditorCamera.GetPosition();
		Vec3 ray_wor = invView * ray_eye;
		ray_wor += posOffset;
		return ray_wor;
	}

	int EditorLayer::GetIDat(Vec2 pos)
	{
		return m_Framebuffer->ReadPixel(1, pos.x, pos.y);
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (!CanPick())
			return false;
		if (e.GetButton() == Mouse::ButtonLeft)
		{
			Vec2 mousePos = GetMouseViewportSpace();
			if (IsInViewportSpace(mousePos))
			{
				int pixelData = GetIDat(mousePos);

				if (pixelData == -1)
				{
					m_HierarchyPanel.SetSelectedSO({});
				}
				m_HierarchyPanel.SetSelectedSO(SceneObject{(entt::entity)pixelData, m_ActiveScene.get()});
			}
		}
		return false;
	}

	void EditorLayer::OnOverlayRender()
	{
		//disable depth test
		glDisable(GL_DEPTH_TEST);
		// TODO: refactor after adding render passes to make them show on top
		// TODO: and the selection outline ttopmost

		Renderer2D::BeginScene(m_EditorCamera);

		if (m_ShowPhysicsColliders)
		{
			//Rendering circle colliders
			{
				auto view = m_ActiveScene->GetSceneObjectsWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [t, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);
					Vec3 position = t.Position() + Vec3(cc2d.Offset, 0.0f);
					Vec3 scale = t.Scale() * cc2d.Radius * 2.0f;

					Mat4 transform = glm::translate(glm::mat4(1.0f), t.WorldPosition())
						* glm::rotate(glm::mat4(1.0f), Radians(t.WorldRotation().z), glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::translate(glm::mat4(1.0f), Vec3(cc2d.Offset, 0.0f))
						* glm::scale(glm::mat4(1.0f), scale);
					float thickness = 0.05f / scale.x * Renderer2D::Draw::GetLineWidth();
					Renderer2D::Draw::Circle(transform, {0.77f, 1.0f, 0.27f, 1.0f}, thickness, 0.0f, -1);
				}
			}
			//Rendering box colliders
			{
				auto view = m_ActiveScene->GetSceneObjectsWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [t, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					Vec3 scale = t.WorldScale() * Vec3(bc2d.Size * 2.0f, 1.0f);

					Mat4 transform = glm::translate(glm::mat4(1.0f), t.WorldPosition())
						* glm::rotate(glm::mat4(1.0f), Radians(t.WorldRotation().z), glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::translate(glm::mat4(1.0f), Vec3(bc2d.Offset, 0.0f))
						* glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::Draw::Rect(transform, {0.77f, 1.0f, 0.27f, 1.0f}, -1);
				}
			}
		}

		// Draw selected sceneObject outline
		SceneObject so = m_HierarchyPanel.GetSelectedSO();
		if (so)
		{
			TransformComponent transform = so.GetComponent<TransformComponent>();
			Renderer2D::Draw::Rect(transform.GetTransform(), {1.0f, 0.6f, 0.23f, 1.0f});
		}


		Renderer2D::EndScene();
		glEnable(GL_DEPTH_TEST);
	}

	void EditorLayer::DuplicateSelected()
	{
		SceneObject so = m_HierarchyPanel.GetSelectedSO();
		if (so)
		{
#if COPY_RUNTIME_SCENE
			m_HierarchyPanel.SetSelectedSO(m_EditorScene->DuplicateSceneObject(so));
#else
			m_HierarchyPanel.SetSelectedSO(m_ActiveScene->DuplicateSceneObject(so));
#endif
		}
	}


#if COPY_RUNTIME_SCENE
	void EditorLayer::NewScene()
	{
		if (m_SceneState == SceneState::Edit)
		{
			m_ActiveScenePath = "";
			m_EditorScene = CreateRef<Scene>();
			m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			
			m_ActiveScene = m_EditorScene;
			m_HierarchyPanel.SetContext(m_ActiveScene);
		}
		else
		{
			MRG_CORE_ERROR("Can't change scene in play mode");
		}
	}
	
	void EditorLayer::OpenScene()
	{
		const char* filter = "Mirage scene (*.mrgs)\0*.mrgs\0";
		const std::string filepath = FileDialogs::OpenFile(filter);
	
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}
	
	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_SceneState == SceneState::Edit)
		{
			if (path.extension().string() != Extensions::scene)
			{
				MRG_WARN("Could not load {0} - not a scene file", path.filename().string());
				return;
			}
			Ref<Scene> newScene = CreateRef<Scene>();
			SceneSerializer serializer(newScene);
			if (serializer.DeserializeText(path))
			{
				m_EditorScene = newScene;
				
				m_ActiveScenePath = path;
				m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				
				m_ActiveScene = m_EditorScene;
				m_HierarchyPanel.SetContext(m_ActiveScene);
			}
		}
		else
		{
			MRG_CORE_ERROR("Can't change scene in play mode");
		}
	}
	
	void EditorLayer::Save()
	{
		if (m_SceneState == SceneState::Edit)
		{
			if (m_ActiveScenePath.empty())
			{
				SaveAs();
			}
			else
			{
				if (m_ActiveScenePath.extension() != Extensions::scene)
				{
					m_ActiveScenePath.append(Extensions::scene);
				}
				SceneSerializer serializer(m_ActiveScene);
				serializer.SerializeText(m_ActiveScenePath);
			}
		}
		else
		{
			MRG_CORE_ERROR("Can't save scene in play mode");
		}
	}
	
	void EditorLayer::SaveAs()
	{
		if (m_SceneState == SceneState::Edit)
		{
			const char* filter = "Mirage scene (*.mrgs)\0*.mrgs\0";
			std::string filepath = FileDialogs::SaveFile(filter);
			if (!filepath.empty())
			{
				// check for extension
				if (std::filesystem::path(filepath).extension() != Extensions::scene)
				{
					filepath.append(Extensions::scene);
				}
				SceneSerializer serializer(m_ActiveScene);
				serializer.SerializeText(filepath);
				m_ActiveScenePath = filepath;
			}
		}
		else
		{
			MRG_CORE_ERROR("Can't save scene in play mode");
		}
	}
	
	void EditorLayer::OnScenePlay()
	{
		// m_ActiveScene = Scene::Copy(m_EditorScene);
		
		const char* filter = "Mirage scene (*.mrgs)\0*.mrgs\0";
		SceneSerializer serializer(m_ActiveScene);
		serializer.SerializeText(Paths::tempScene);
	
		
		m_HierarchyPanel.SetSelectedSO({});
		
		m_SceneState = SceneState::Play;
		m_HierarchyPanel.SetContext(m_ActiveScene);
		m_ActiveScene->OnRuntimeStart();
	}
	
	void EditorLayer::OnSceneStop()
	{
		m_ActiveScene->OnRuntimeStop();
	
		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.DeserializeText(Paths::tempScene))
		{
			m_EditorScene = newScene;
				
			m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				
			m_ActiveScene = m_EditorScene;
			m_SceneState = SceneState::Edit;
			m_HierarchyPanel.SetContext(m_ActiveScene);
		}
	}
#else
	void EditorLayer::NewScene()
	{
		if (m_SceneState == SceneState::Edit)
		{
			m_ActiveScenePath = "";
			m_ActiveScene = CreateRef<Scene>();
			// m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_HierarchyPanel.SetContext(m_ActiveScene);
		}
		else
		{
			MRG_CORE_ERROR("Can't change scene in play mode");
		}
	}

	void EditorLayer::OpenScene()
	{
		const char* filter = "Mirage scene (*.mrgs)\0*.mrgs\0";
		const std::string filepath = FileDialogs::OpenFile(filter);

		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_SceneState == SceneState::Edit)
		{
			if (path.extension().string() != Extensions::scene)
			{
				MRG_WARN("Could not load {0} - not a scene file", path.filename().string());
				return;
			}
			Ref<Scene> newScene = CreateRef<Scene>();
			SceneSerializer serializer(newScene);
			if (serializer.DeserializeText(path))
			{
				m_ActiveScene = newScene;

				m_ActiveScenePath = path;
				// m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

				m_HierarchyPanel.SetContext(m_ActiveScene);
			}
		}
		else
		{
			MRG_CORE_ERROR("Can't change scene in play mode");
		}
	}

	void EditorLayer::Save()
	{
		if (m_SceneState == SceneState::Edit)
		{
			if (m_ActiveScenePath.empty())
			{
				SaveAs();
			}
			else
			{
				if (m_ActiveScenePath.extension() != Extensions::scene)
				{
					m_ActiveScenePath.append(Extensions::scene);
				}
				SceneSerializer serializer(m_ActiveScene);
				serializer.SerializeText(m_ActiveScenePath);
			}
		}
		else
		{
			MRG_CORE_ERROR("Can't save scene in play mode");
		}
	}

	void EditorLayer::SaveAs()
	{
		if (m_SceneState == SceneState::Edit)
		{
			const char* filter = "Mirage scene (*.mrgs)\0*.mrgs\0";
			std::string filepath = FileDialogs::SaveFile(filter);
			if (!filepath.empty())
			{
				// check for extension
				if (std::filesystem::path(filepath).extension() != Extensions::scene)
				{
					filepath.append(Extensions::scene);
				}
				SceneSerializer serializer(m_ActiveScene);
				serializer.SerializeText(filepath);
				m_ActiveScenePath = filepath;
			}
		}
		else
		{
			MRG_CORE_ERROR("Can't save scene in play mode");
		}
	}

	void EditorLayer::OnScenePlay()
	{
		// m_ActiveScene = Scene::Copy(m_EditorScene);

		// const char* filter = "Mirage scene (*.mrgs)\0*.mrgs\0";
		SceneSerializer serializer(m_ActiveScene);
		serializer.SerializeText(Paths::TempScene);


		m_HierarchyPanel.SetSelectedSO({});

		m_SceneState = SceneState::Play;
		m_HierarchyPanel.SetContext(m_ActiveScene);
		m_ActiveScene->OnRuntimeStart();

		// for (int i = 0; i < 100000; ++i)
		// {
		// 	SceneObject so = m_ActiveScene->CreateSceneObject();
		// 	so.AddComponent<SpriteRendererComponent>();
		// }
	}

	void EditorLayer::OnSceneStop()
	{
		MRG_CORE_ASSERT(m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate, "Scene state Unknown")

		if (m_SceneState == SceneState::Play)
		{
			m_ActiveScene->OnRuntimeStop();
		}
		else if (m_SceneState == SceneState::Simulate)
		{
			m_ActiveScene->OnSimulationStop();
		}

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.DeserializeText(Paths::TempScene))
		{
			m_ActiveScene = newScene;

			m_HierarchyPanel.SetSelectedSO({});

			m_SceneState = SceneState::Edit;
			m_HierarchyPanel.SetContext(m_ActiveScene);
		}
	}

	void EditorLayer::OnSceneSimulate()
	{
		// m_ActiveScene = Scene::Copy(m_EditorScene);

		// const char* filter = "Mirage scene (*.mrgs)\0*.mrgs\0";
		SceneSerializer serializer(m_ActiveScene);
		serializer.SerializeText(Paths::TempScene);


		m_HierarchyPanel.SetSelectedSO({});

		m_SceneState = SceneState::Simulate;
		m_HierarchyPanel.SetContext(m_ActiveScene);
		m_ActiveScene->OnSimulationStart();
	}
#endif
}

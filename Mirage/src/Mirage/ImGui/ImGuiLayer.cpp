#include "MrgPch.h"
#include "ImGuiLayer.h"

#include "Mirage/Application.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

//Temporary
#include <glad/glad.h>
#include "Mirage/KeyCodes.h"
#include "Mirage/MouseButtonCodes.h"

namespace Mirage
{
    void SetImGuiStyle()
    {
        ImGuiStyle* style = &ImGui::GetStyle();

        style->WindowPadding = ImVec2(9, 9);
        style->FramePadding = ImVec2(5, 5);
        style->ItemSpacing = ImVec2(10, 7);
        style->ItemInnerSpacing = ImVec2(7, 7);
        style->TouchExtraPadding = ImVec2(0, 0);
        style->IndentSpacing = 20.0f;
        style->ScrollbarSize = 15.0f;
        style->GrabMinSize = 5.0f;

        style->WindowBorderSize = 1.0f;
        style->ChildBorderSize = 1.0f;
        style->PopupBorderSize = 1.0f;
        style->FrameBorderSize = 0.0f;

        style->WindowRounding = 3.0f;
        style->ChildRounding = 3.0f;
        style->FrameRounding = 3.0f;
        style->ScrollbarRounding = 12.0f;
        style->GrabRounding = 3.0f;

        style->WindowTitleAlign = ImVec2(0.0f, 0.5f);
        style->ButtonTextAlign = ImVec2(0.5f, 0.5f);

        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.25f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.02f, 0.02f, 0.03f, 1.00f);
        colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.17f, 0.17f, 0.40f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.38f, 0.38f, 0.69f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.09f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.41f, 0.41f, 0.41f, 0.40f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.80f, 0.31f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.80f, 0.80f, 0.90f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.46f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.67f, 0.67f, 0.67f, 1.00f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.08f, 0.04f, 0.61f, 0.72f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.36f, 0.72f, 0.95f, 0.72f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.28f, 0.76f, 0.95f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.89f, 0.89f, 0.89f, 0.63f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.89f, 0.89f, 0.89f, 0.63f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.06f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.12f, 0.22f, 0.44f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.16f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.71f);
    }

    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        ImGui::CreateContext();
        SetImGuiStyle();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        //Temporary: Change to mirage mapping

        io.KeyMap[ImGuiKey_Tab]         = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_TAB);
        io.KeyMap[ImGuiKey_LeftArrow]   = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_LEFT);
        io.KeyMap[ImGuiKey_RightArrow]  = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_RIGHT);
        io.KeyMap[ImGuiKey_UpArrow]     = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_UP);
        io.KeyMap[ImGuiKey_DownArrow]   = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_DOWN);
        io.KeyMap[ImGuiKey_PageUp]      = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_PAGE_UP);
        io.KeyMap[ImGuiKey_PageDown]    = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_PAGE_DOWN);
        io.KeyMap[ImGuiKey_Home]        = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_HOME);
        io.KeyMap[ImGuiKey_End]         = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_END);
        io.KeyMap[ImGuiKey_Insert]      = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_INSERT);
        io.KeyMap[ImGuiKey_Delete]      = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_DELETE);
        io.KeyMap[ImGuiKey_Backspace]   = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_BACKSPACE);
        io.KeyMap[ImGuiKey_Space]       = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_SPACE);
        io.KeyMap[ImGuiKey_Enter]       = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_ENTER);
        io.KeyMap[ImGuiKey_KeypadEnter] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_ENTER);
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_ENTER);
        io.KeyMap[ImGuiKey_Escape]      = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_ESCAPE);

        if (PRIMARYLANGID(HIWORD(GetKeyboardLayout(0))) == LANG_FRENCH)
        {
            io.KeyMap[ImGuiKey_A] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_Q);
            io.KeyMap[ImGuiKey_C] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_C);
            io.KeyMap[ImGuiKey_V] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_V);
            io.KeyMap[ImGuiKey_X] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_X);
            io.KeyMap[ImGuiKey_Y] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_Y);
            io.KeyMap[ImGuiKey_Z] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_W);
        }
        else
        {
            MRG_CORE_ERROR("Keyboard layout not supported, Falling Back to English layout");
            io.KeyMap[ImGuiKey_A] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_A);
            io.KeyMap[ImGuiKey_C] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_C);
            io.KeyMap[ImGuiKey_V] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_V);
            io.KeyMap[ImGuiKey_X] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_X);
            io.KeyMap[ImGuiKey_Y] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_Y);
            io.KeyMap[ImGuiKey_Z] = GLFW_KEY_TO_MRG_KEY(GLFW_KEY_Z);
        }

        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
    }

    void ImGuiLayer::OnUpdate()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(MRG_BIND_EVENT_FN(&ImGuiLayer::OnMouseButtonPressed));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(MRG_BIND_EVENT_FN(&ImGuiLayer::OnMouseButtonReleased));
        dispatcher.Dispatch<MouseMoveEvent>(MRG_BIND_EVENT_FN(&ImGuiLayer::OnMouseMoved));
        dispatcher.Dispatch<MouseScrollEvent>(MRG_BIND_EVENT_FN(&ImGuiLayer::OnMouseScroll));

        dispatcher.Dispatch<KeyPressedEvent>(MRG_BIND_EVENT_FN(&ImGuiLayer::OnKeyPressed));
        dispatcher.Dispatch<KeyTypedEvent>(MRG_BIND_EVENT_FN(&ImGuiLayer::OnKeyTyped));
        dispatcher.Dispatch<KeyReleasedEvent>(MRG_BIND_EVENT_FN(&ImGuiLayer::OnKeyReleased));

        dispatcher.Dispatch<WindowResizeEvent>(MRG_BIND_EVENT_FN(&ImGuiLayer::OnWindowResize));
    }

    bool ImGuiLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.GetButton()] = true;

        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.GetButton()] = false;

        return false;
    }

    bool ImGuiLayer::OnMouseMoved(MouseMoveEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(event.GetX(), event.GetY());

        return false;
    }

    bool ImGuiLayer::OnMouseScroll(MouseScrollEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();

        io.MouseWheelH += event.GetXOffset();
        io.MouseWheel += event.GetYOffset();

        return false;
    }

    bool ImGuiLayer::OnKeyPressed(KeyPressedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[MRG_Key_LeftControl] || io.KeysDown[MRG_Key_RightControl];
        io.KeyShift = io.KeysDown[MRG_Key_LeftShift] || io.KeysDown[MRG_Key_RightShift];
        io.KeyAlt = io.KeysDown[MRG_Key_LeftAlt] || io.KeysDown[MRG_Key_RightAlt];
        io.KeySuper = io.KeysDown[MRG_Key_LeftSuper] || io.KeysDown[MRG_Key_RightSuper];

        return false;
    }

    bool ImGuiLayer::OnKeyReleased(KeyReleasedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.GetKeyCode()] = false;

        io.KeyCtrl = io.KeysDown[MRG_Key_LeftControl] || io.KeysDown[MRG_Key_RightControl];
        io.KeyShift = io.KeysDown[MRG_Key_LeftShift] || io.KeysDown[MRG_Key_RightShift];
        io.KeyAlt = io.KeysDown[MRG_Key_LeftAlt] || io.KeysDown[MRG_Key_RightAlt];
        io.KeySuper = io.KeysDown[MRG_Key_LeftSuper] || io.KeysDown[MRG_Key_RightSuper];

        return false;
    }

    bool ImGuiLayer::OnKeyTyped(KeyTypedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();

        int keyCode = event.GetKeyCode();
        if (keyCode > 0 && keyCode < 0x10000)
            io.AddInputCharacter((unsigned short)keyCode);

        return false;
    }

    bool ImGuiLayer::OnWindowResize(WindowResizeEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();

        io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, event.GetWidth(), event.GetHeight());

        return false;
    }
}

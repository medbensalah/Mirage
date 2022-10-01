#include "MrgPch.h"
#include "Mirage/ImGui/ImGuiLayer.h"

#define IMGUI_IMPL_API

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "Mirage/Core/Application.h"

namespace Mirage
{
    void SetImGuiStyle(bool dark = true)
    {
        ImGuiStyle* style = &ImGui::GetStyle();

        style->WindowPadding                        = ImVec2(7, 7);
        style->FramePadding                         = ImVec2(7, 2);
        style->ItemSpacing                          = ImVec2(12, 7);
        style->ItemInnerSpacing                     = ImVec2(7, 7);
        style->TouchExtraPadding                    = ImVec2(0, 0);
        style->IndentSpacing                        = 22.0f;
        style->ScrollbarSize                        = 10.0f;
        style->GrabMinSize                          = 3.0f;

        //Borders
        style->WindowBorderSize                     = 1.0f;
        style->ChildBorderSize                      = 1.0f;
        style->PopupBorderSize                      = 1.0f;
        style->FrameBorderSize                      = 0.0f;
        style->TabBorderSize                        = 1.0f;

        //Rounding
        style->WindowRounding                       = 10.0f;
        style->ChildRounding                        = 10.0f;
        style->FrameRounding                        = 5.0f;
        style->PopupRounding                        = 10.0f;
        style->ScrollbarRounding                    = 10.0f;
        style->GrabRounding                         = 5.0f;
        style->LogSliderDeadzone                    = 5.0f;
        style->TabRounding                          = 5.0f;

        //Alignment
        style->WindowTitleAlign                     = ImVec2(0.02f, 0.02f);
        style->WindowMenuButtonPosition             = ImGuiDir_Left;
        style->ColorButtonPosition                  = ImGuiDir_Left;
        style->ButtonTextAlign                      = ImVec2(0.5f, 0.5f);

        //Safe area
        style->DisplaySafeAreaPadding               = ImVec2(5, 5);
                
        ImVec4* colors = ImGui::GetStyle().Colors;
        if(dark)
        {
            colors[ImGuiCol_Text]                   = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
            colors[ImGuiCol_TextDisabled]           = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
            colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
            colors[ImGuiCol_ChildBg]                = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
            colors[ImGuiCol_PopupBg]                = ImVec4(0.04f, 0.04f, 0.04f, 0.89f);
            colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.71f);
            colors[ImGuiCol_FrameBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.00f, 0.00f, 0.49f, 1.00f);
            colors[ImGuiCol_FrameBgActive]          = ImVec4(0.00f, 0.00f, 0.35f, 1.00f);
            colors[ImGuiCol_TitleBg]                = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
            colors[ImGuiCol_TitleBgActive]          = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
            colors[ImGuiCol_MenuBarBg]              = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
            colors[ImGuiCol_ScrollbarBg]            = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
            colors[ImGuiCol_ScrollbarGrab]          = ImVec4(1.00f, 1.00f, 1.00f, 0.13f);
            colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.80f, 0.80f, 0.80f, 0.90f);
            colors[ImGuiCol_CheckMark]              = ImVec4(0.69f, 0.69f, 0.70f, 1.00f);
            colors[ImGuiCol_SliderGrab]             = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
            colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
            colors[ImGuiCol_Button]                 = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_ButtonHovered]          = ImVec4(0.00f, 0.00f, 0.49f, 1.00f);
            colors[ImGuiCol_ButtonActive]           = ImVec4(0.00f, 0.00f, 0.35f, 1.00f);
            colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.49f, 1.00f);
            colors[ImGuiCol_HeaderActive]           = ImVec4(0.00f, 0.00f, 0.35f, 0.88f);
            colors[ImGuiCol_Separator]              = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.00f, 0.00f, 0.49f, 1.00f);
            colors[ImGuiCol_SeparatorActive]        = ImVec4(0.00f, 0.00f, 0.35f, 1.00f);
            colors[ImGuiCol_ResizeGrip]             = ImVec4(0.02f, 0.00f, 0.31f, 1.00f);
            colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.00f, 0.00f, 0.49f, 1.00f);
            colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.00f, 0.00f, 0.35f, 1.00f);
            colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TabHovered]             = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
            colors[ImGuiCol_TabActive]              = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
            colors[ImGuiCol_TabUnfocused]           = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
            colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
            colors[ImGuiCol_DockingPreview]         = ImVec4(0.00f, 0.00f, 0.49f, 1.00f);
            colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
            colors[ImGuiCol_PlotLines]              = ImVec4(0.89f, 0.89f, 0.89f, 0.63f);
            colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogram]          = ImVec4(0.89f, 0.89f, 0.89f, 0.63f);
            colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TableBorderLight]       = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.06f);
            colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
            colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.00f, 0.00f, 0.35f, 1.00f);
            colors[ImGuiCol_DragDropTarget]         = ImVec4(0.09f, 0.54f, 0.00f, 1.00f);
            colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.00f, 0.00f, 0.00f, 0.71f);
        }
        else
        {
            colors[ImGuiCol_Text]                   = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
            colors[ImGuiCol_TextDisabled]           = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
            colors[ImGuiCol_WindowBg]               = ImVec4(0.81f, 0.86f, 0.91f, 1.00f);
            colors[ImGuiCol_ChildBg]                = ImVec4(0.66f, 0.76f, 0.78f, 0.31f);
            colors[ImGuiCol_PopupBg]                = ImVec4(0.88f, 0.92f, 1.00f, 0.87f);
            colors[ImGuiCol_Border]                 = ImVec4(0.85f, 0.97f, 1.00f, 1.00f);
            colors[ImGuiCol_BorderShadow]           = ImVec4(0.32f, 0.32f, 0.32f, 0.71f);
            colors[ImGuiCol_FrameBg]                = ImVec4(0.88f, 0.96f, 0.97f, 1.00f);
            colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.66f, 0.66f, 1.00f, 1.00f);
            colors[ImGuiCol_FrameBgActive]          = ImVec4(0.35f, 0.35f, 0.78f, 1.00f);
            colors[ImGuiCol_TitleBg]                = ImVec4(0.25f, 0.27f, 0.85f, 0.04f);
            colors[ImGuiCol_TitleBgActive]          = ImVec4(0.25f, 0.27f, 0.85f, 0.24f);
            colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.83f, 0.84f, 1.00f, 0.70f);
            colors[ImGuiCol_MenuBarBg]              = ImVec4(0.81f, 0.91f, 0.96f, 0.82f);
            colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.71f, 0.77f, 0.80f, 0.79f);
            colors[ImGuiCol_ScrollbarGrab]          = ImVec4(1.00f, 1.00f, 1.00f, 0.69f);
            colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(1.00f, 1.00f, 1.00f, 0.69f);
            colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(1.00f, 1.00f, 1.00f, 0.69f);
            colors[ImGuiCol_CheckMark]              = ImVec4(0.32f, 0.25f, 1.00f, 1.00f);
            colors[ImGuiCol_SliderGrab]             = ImVec4(0.33f, 0.25f, 1.00f, 1.00f);
            colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.33f, 0.25f, 1.00f, 1.00f);
            colors[ImGuiCol_Button]                 = ImVec4(0.93f, 0.98f, 1.00f, 1.00f);
            colors[ImGuiCol_ButtonHovered]          = ImVec4(0.66f, 0.66f, 1.00f, 1.00f);
            colors[ImGuiCol_ButtonActive]           = ImVec4(0.35f, 0.35f, 0.78f, 1.00f);
            colors[ImGuiCol_Header]                 = ImVec4(0.83f, 0.93f, 1.00f, 1.00f);
            colors[ImGuiCol_HeaderHovered]          = ImVec4(0.66f, 0.66f, 1.00f, 1.00f);
            colors[ImGuiCol_HeaderActive]           = ImVec4(0.35f, 0.35f, 0.78f, 1.00f);
            colors[ImGuiCol_Separator]              = ImVec4(0.65f, 0.68f, 0.72f, 0.50f);
            colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.66f, 0.66f, 1.00f, 1.00f);
            colors[ImGuiCol_SeparatorActive]        = ImVec4(0.35f, 0.35f, 0.78f, 1.00f);
            colors[ImGuiCol_ResizeGrip]             = ImVec4(0.61f, 0.58f, 1.00f, 1.00f);
            colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.66f, 0.66f, 1.00f, 1.00f);
            colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.35f, 0.35f, 0.78f, 1.00f);
            colors[ImGuiCol_Tab]                    = ImVec4(0.74f, 0.74f, 0.97f, 1.00f);
            colors[ImGuiCol_TabHovered]             = ImVec4(0.66f, 0.66f, 1.00f, 1.00f);
            colors[ImGuiCol_TabActive]              = ImVec4(0.47f, 0.47f, 0.93f, 0.75f);
            colors[ImGuiCol_TabUnfocused]           = ImVec4(0.47f, 0.47f, 0.93f, 1.00f);
            colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.47f, 0.47f, 0.93f, 1.00f);
            colors[ImGuiCol_DockingPreview]         = ImVec4(0.62f, 0.62f, 0.98f, 1.00f);
            colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.81f, 0.86f, 0.91f, 1.00f);
            colors[ImGuiCol_PlotLines]              = ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.66f, 0.66f, 1.00f, 1.00f);
            colors[ImGuiCol_PlotHistogram]          = ImVec4(0.58f, 0.58f, 1.00f, 0.63f);
            colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.86f, 0.86f, 1.00f, 1.00f);
            colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.81f, 0.90f, 1.00f, 1.00f);
            colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.63f, 0.63f, 0.63f, 1.00f);
            colors[ImGuiCol_TableBorderLight]       = ImVec4(0.89f, 0.89f, 0.89f, 1.00f);
            colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.06f);
            colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.27f);
            colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.59f, 0.59f, 0.99f, 1.00f);
            colors[ImGuiCol_DragDropTarget]         = ImVec4(0.30f, 1.00f, 0.16f, 1.00f);
            colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.00f, 0.00f, 0.00f, 0.71f);
        }


        ImGuiIO& io = ImGui::GetIO();

        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Lato/Lato-Regular.ttf", 16.0f);
                
        ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoSidePreview |
            ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel |
            ImGuiColorEditFlags_NoSidePreview ;
        
        ImGui::SetColorEditOptions(flags);
    }

    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
        MRG_PROFILE_FUNCTION();
    }
    
    void ImGuiLayer::OnAttach()
    {
        MRG_PROFILE_FUNCTION();
        
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;
        
        // Setup Dear ImGui style
        SetImGuiStyle(false);

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        // ImGuiStyle& style = ImGui::GetStyle();
        // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        // {
        //     style.WindowRounding = 0.0f;
        //     style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        // }

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        //setup platform / Renderer binding

        //Temporary: Change to mirage mapping
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
        MRG_PROFILE_FUNCTION();
        
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    
    void ImGuiLayer::OnEvent(Event& e)
    {
        // ImGuiIO& io = ImGui::GetIO();
        // e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
        // e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
    
    void ImGuiLayer::Begin()
    {
        MRG_PROFILE_FUNCTION();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    
    void ImGuiLayer::End()
    {
        MRG_PROFILE_FUNCTION();
        
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        //Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}

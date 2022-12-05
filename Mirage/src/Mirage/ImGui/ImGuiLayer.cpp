#include "MrgPch.h"
#include "Mirage/ImGui/ImGuiLayer.h"

#define IMGUI_IMPL_API

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "Mirage/Core/Application.h"

#include "ImGuizmo.h"

namespace Mirage
{
static bool isDark = true;
    
    void SetImGuiStyle(bool dark = true)
    {
        ImGuiStyle* style = &ImGui::GetStyle();
        
        style->WindowPadding                        = ImVec2(7, 5);
        style->FramePadding                         = ImVec2(7, 3);
        style->CellPadding                         = ImVec2(7, 0);
        style->ItemSpacing                          = ImVec2(7, 3);
        style->ItemInnerSpacing                     = ImVec2(5, 5); 
        style->TouchExtraPadding                    = ImVec2(0, 0);
        
        style->IndentSpacing                        = 15.0f;
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
        style->ChildRounding                        = 0.0f;
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
            colors[ImGuiCol_Text]                   = ImVec4(0.870f, 0.870f, 0.870f, 1.000f);
            colors[ImGuiCol_TextDisabled]           = ImVec4(0.540f, 0.540f, 0.540f, 1.000f);
            colors[ImGuiCol_WindowBg]               = ImVec4(0.100f, 0.100f, 0.100f, 1.000f);
            colors[ImGuiCol_ChildBg]                = ImVec4(0.150f, 0.150f, 0.150f, 1.000f);
            colors[ImGuiCol_PopupBg]                = ImVec4(0.150f, 0.150f, 0.150f, 0.900f);
            colors[ImGuiCol_Border]                 = ImVec4(0.040f, 0.040f, 0.040f, 1.000f);
            colors[ImGuiCol_BorderShadow]           = ImVec4(0.000f, 0.000f, 0.000f, 0.710f);
            colors[ImGuiCol_FrameBg]                = ImVec4(0.062f, 0.062f, 0.062f, 1.000f);
            colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.000f, 0.000f, 0.490f, 1.000f);
            colors[ImGuiCol_FrameBgActive]          = ImVec4(0.000f, 0.000f, 0.350f, 1.000f);
            colors[ImGuiCol_TitleBg]                = ImVec4(0.130f, 0.130f, 0.130f, 1.000f);
            colors[ImGuiCol_TitleBgActive]          = ImVec4(0.060f, 0.060f, 0.060f, 1.000f);
            colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.130f, 0.130f, 0.130f, 1.000f);
            colors[ImGuiCol_MenuBarBg]              = ImVec4(0.130f, 0.130f, 0.130f, 1.000f);
            colors[ImGuiCol_ScrollbarBg]            = ImVec4(1.000f, 1.000f, 1.000f, 0.070f);
            colors[ImGuiCol_ScrollbarGrab]          = ImVec4(1.000f, 1.000f, 1.000f, 0.130f);
            colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.560f, 0.560f, 0.560f, 1.000f);
            colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.800f, 0.800f, 0.800f, 0.900f);
            colors[ImGuiCol_CheckMark]              = ImVec4(0.690f, 0.690f, 0.700f, 1.000f);
            colors[ImGuiCol_SliderGrab]             = ImVec4(0.000f, 0.000f, 0.490f, 1.000f);
            colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.000f, 0.000f, 0.350f, 1.000f);
            colors[ImGuiCol_Button]                 = ImVec4(0.060f, 0.060f, 0.060f, 1.000f);
            colors[ImGuiCol_ButtonHovered]          = ImVec4(0.000f, 0.000f, 0.490f, 1.000f);
            colors[ImGuiCol_ButtonActive]           = ImVec4(0.000f, 0.000f, 0.350f, 1.000f);
            colors[ImGuiCol_GradButtonU]            = ImVec4(0.230f, 0.230f, 0.230f, 1.000f);
            colors[ImGuiCol_GradButtonHoveredU]     = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
            colors[ImGuiCol_GradButtonActiveU]      = ImVec4(0.050f, 0.050f, 0.050f, 1.000f);
            colors[ImGuiCol_GradButtonD]            = ImVec4(0.030f, 0.030f, 0.030f, 1.000f);
            colors[ImGuiCol_GradButtonHoveredD]     = ImVec4(0.050f, 0.050f, 0.050f, 1.000f);
            colors[ImGuiCol_GradButtonActiveD]      = ImVec4(0.140f, 0.140f, 0.140f, 1.000f);
            colors[ImGuiCol_Header]                 = ImVec4(0.030f, 0.030f, 0.030f, 1.000f);
            colors[ImGuiCol_HeaderHovered]          = ImVec4(0.000f, 0.000f, 0.490f, 1.000f);
            colors[ImGuiCol_HeaderActive]           = ImVec4(0.000f, 0.000f, 0.350f, 0.880f);
            colors[ImGuiCol_Separator]              = ImVec4(0.060f, 0.060f, 0.060f, 1.000f);
            colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.000f, 0.000f, 0.490f, 1.000f);
            colors[ImGuiCol_SeparatorActive]        = ImVec4(0.000f, 0.000f, 0.350f, 1.000f);
            colors[ImGuiCol_ResizeGrip]             = ImVec4(0.020f, 0.000f, 0.310f, 1.000f);
            colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.000f, 0.000f, 0.490f, 1.000f);
            colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.000f, 0.000f, 0.350f, 1.000f);
            colors[ImGuiCol_Tab]                    = ImVec4(0.060f, 0.060f, 0.060f, 1.000f);
            colors[ImGuiCol_TabHovered]             = ImVec4(0.290f, 0.290f, 0.290f, 1.000f);
            colors[ImGuiCol_TabActive]              = ImVec4(0.220f, 0.220f, 0.220f, 1.000f);
            colors[ImGuiCol_TabUnfocused]           = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
            colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
            colors[ImGuiCol_DockingPreview]         = ImVec4(0.000f, 0.000f, 0.490f, 1.000f);
            colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.100f, 0.100f, 0.100f, 1.000f);
            colors[ImGuiCol_PlotLines]              = ImVec4(0.890f, 0.890f, 0.890f, 0.630f);
            colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.250f, 1.000f, 0.000f, 1.000f);
            colors[ImGuiCol_PlotHistogram]          = ImVec4(0.890f, 0.890f, 0.890f, 0.630f);
            colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.250f, 1.000f, 0.000f, 1.000f);
            colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.060f, 0.060f, 0.060f, 1.000f);
            colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.060f, 0.060f, 0.060f, 1.000f);
            colors[ImGuiCol_TableBorderLight]       = ImVec4(0.210f, 0.210f, 0.210f, 1.000f);
            colors[ImGuiCol_TableRowBg]             = ImVec4(0.000f, 0.000f, 0.000f, 0.060f);
            colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.000f, 1.000f, 1.000f, 0.060f);
            colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.000f, 0.000f, 0.350f, 1.000f);
            colors[ImGuiCol_DragDropTarget]         = ImVec4(0.090f, 0.540f, 0.000f, 1.000f);
            colors[ImGuiCol_NavHighlight]           = ImVec4(0.260f, 0.590f, 0.980f, 1.000f);
            colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.000f, 1.000f, 1.000f, 0.700f);
            colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.800f, 0.800f, 0.800f, 0.200f);
            colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.000f, 0.000f, 0.000f, 0.710f);
        }
        else
        {
            colors[ImGuiCol_Text]                   = ImVec4(0.050f, 0.050f, 0.050f, 1.000f);
            colors[ImGuiCol_TextDisabled]           = ImVec4(0.540f, 0.540f, 0.540f, 1.000f);
            colors[ImGuiCol_WindowBg]               = ImVec4(0.810f, 0.860f, 0.910f, 1.000f);
            colors[ImGuiCol_ChildBg]                = ImVec4(0.660f, 0.760f, 0.780f, 0.310f);
            colors[ImGuiCol_PopupBg]                = ImVec4(0.880f, 0.920f, 1.000f, 0.870f);
            colors[ImGuiCol_Border]                 = ImVec4(0.850f, 0.970f, 1.000f, 1.000f);
            colors[ImGuiCol_BorderShadow]           = ImVec4(0.320f, 0.320f, 0.320f, 0.710f);
            colors[ImGuiCol_FrameBg]                = ImVec4(0.880f, 0.960f, 0.970f, 1.000f);
            colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.660f, 0.660f, 1.000f, 1.000f);
            colors[ImGuiCol_FrameBgActive]          = ImVec4(0.350f, 0.350f, 0.780f, 1.000f);
            colors[ImGuiCol_TitleBg]                = ImVec4(0.250f, 0.270f, 0.850f, 0.040f);
            colors[ImGuiCol_TitleBgActive]          = ImVec4(0.250f, 0.270f, 0.850f, 0.240f);
            colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.830f, 0.840f, 1.000f, 0.700f);
            colors[ImGuiCol_MenuBarBg]              = ImVec4(0.810f, 0.910f, 0.960f, 0.820f);
            colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.710f, 0.770f, 0.800f, 0.790f);
            colors[ImGuiCol_ScrollbarGrab]          = ImVec4(1.000f, 1.000f, 1.000f, 0.690f);
            colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(1.000f, 1.000f, 1.000f, 0.690f);
            colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(1.000f, 1.000f, 1.000f, 0.690f);
            colors[ImGuiCol_CheckMark]              = ImVec4(0.320f, 0.250f, 1.000f, 1.000f);
            colors[ImGuiCol_SliderGrab]             = ImVec4(0.330f, 0.250f, 1.000f, 1.000f);
            colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.330f, 0.250f, 1.000f, 1.000f);
            colors[ImGuiCol_Button]                 = ImVec4(0.930f, 0.980f, 1.000f, 1.000f);
            colors[ImGuiCol_ButtonHovered]          = ImVec4(0.660f, 0.660f, 1.000f, 1.000f);
            colors[ImGuiCol_ButtonActive]           = ImVec4(0.350f, 0.350f, 0.780f, 1.000f);
            colors[ImGuiCol_GradButtonU]            = ImVec4(0.930f, 0.980f, 1.000f, 1.000f);
            colors[ImGuiCol_GradButtonHoveredU]     = ImVec4(0.890f, 0.890f, 1.000f, 1.000f);
            colors[ImGuiCol_GradButtonActiveU]      = ImVec4(0.620f, 0.620f, 0.920f, 1.000f);
            colors[ImGuiCol_GradButtonD]            = ImVec4(0.730f, 0.780f, 1.000f, 1.000f);
            colors[ImGuiCol_GradButtonHoveredD]     = ImVec4(0.620f, 0.620f, 0.920f, 1.000f);
            colors[ImGuiCol_GradButtonActiveD]      = ImVec4(0.890f, 0.890f, 1.000f, 1.000f);
            colors[ImGuiCol_Header]                 = ImVec4(0.830f, 0.930f, 1.000f, 1.000f);
            colors[ImGuiCol_HeaderHovered]          = ImVec4(0.660f, 0.660f, 1.000f, 1.000f);
            colors[ImGuiCol_HeaderActive]           = ImVec4(0.350f, 0.350f, 0.780f, 1.000f);
            colors[ImGuiCol_Separator]              = ImVec4(0.650f, 0.680f, 0.720f, 0.500f);
            colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.660f, 0.660f, 1.000f, 1.000f);
            colors[ImGuiCol_SeparatorActive]        = ImVec4(0.350f, 0.350f, 0.780f, 1.000f);
            colors[ImGuiCol_ResizeGrip]             = ImVec4(0.610f, 0.580f, 1.000f, 1.000f);
            colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.660f, 0.660f, 1.000f, 1.000f);
            colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.350f, 0.350f, 0.780f, 1.000f);
            colors[ImGuiCol_Tab]                    = ImVec4(0.740f, 0.740f, 0.970f, 1.000f);
            colors[ImGuiCol_TabHovered]             = ImVec4(0.660f, 0.660f, 1.000f, 1.000f);
            colors[ImGuiCol_TabActive]              = ImVec4(0.470f, 0.470f, 0.930f, 0.750f);
            colors[ImGuiCol_TabUnfocused]           = ImVec4(0.470f, 0.470f, 0.930f, 1.000f);
            colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.470f, 0.470f, 0.930f, 1.000f);
            colors[ImGuiCol_DockingPreview]         = ImVec4(0.620f, 0.620f, 0.980f, 1.000f);
            colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.810f, 0.860f, 0.910f, 1.000f);
            colors[ImGuiCol_PlotLines]              = ImVec4(0.580f, 0.580f, 0.580f, 1.000f);
            colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.660f, 0.660f, 1.000f, 1.000f);
            colors[ImGuiCol_PlotHistogram]          = ImVec4(0.580f, 0.580f, 1.000f, 0.630f);
            colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.860f, 0.860f, 1.000f, 1.000f);
            colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.810f, 0.900f, 1.000f, 1.000f);
            colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.630f, 0.630f, 0.630f, 1.000f);
            colors[ImGuiCol_TableBorderLight]       = ImVec4(0.890f, 0.890f, 0.890f, 1.000f);
            colors[ImGuiCol_TableRowBg]             = ImVec4(0.000f, 0.000f, 0.000f, 0.060f);
            colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.000f, 1.000f, 1.000f, 0.270f);
            colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.590f, 0.590f, 0.990f, 1.000f);
            colors[ImGuiCol_DragDropTarget]         = ImVec4(0.300f, 1.000f, 0.160f, 1.000f);
            colors[ImGuiCol_NavHighlight]           = ImVec4(0.260f, 0.590f, 0.980f, 1.000f);
            colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.000f, 1.000f, 1.000f, 0.700f);
            colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.800f, 0.800f, 0.800f, 0.200f);
            colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.000f, 0.000f, 0.000f, 0.710f);
        }


        ImGuiIO& io = ImGui::GetIO();
        
        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Lato/Lato-Regular.ttf", 18.0f);
        io.Fonts->AddFontFromFileTTF("assets/fonts/Lato/Lato-Black.ttf", 18.0f);

        
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
        // io.ConfigViewportsNoAutoMerge = true;
        io.ConfigViewportsNoTaskBarIcon = true;
        
        // Setup Dear ImGui style
        SetImGuiStyle(isDark);

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
        ImGui_ImplOpenGL3_Init("#version 450");
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
        ImGuiIO& io = ImGui::GetIO();

        if(m_BlockKbEvents)
            e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        if(m_BlockMouseEvents)
            e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
    }
    
    void ImGuiLayer::Begin()
    {
        MRG_PROFILE_FUNCTION();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
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

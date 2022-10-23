#pragma once

namespace Mirage
{
        enum class Font
        {
            Regular     = 0,
            Black       = 1,
            Italic      = 2
        };

    static ImFont* GetFont(Font style)
    {
        ImGuiIO& io = ImGui::GetIO();
       return io.Fonts->Fonts[(int)style];
    }
}

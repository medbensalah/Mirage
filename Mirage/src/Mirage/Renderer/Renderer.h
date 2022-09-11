#pragma once

namespace Mirage
{
    enum class GraphicsAPI
    {
        None = 0,
        OpenGL = 1
    };

    MIRAGE_CLASS Renderer
    {
    public:
        inline static GraphicsAPI GetGfxApi() { return s_GfxApi; }
    private:
        static GraphicsAPI s_GfxApi;
    };
}

#pragma once

namespace Mirage
{
    MIRAGE_CLASS GraphicsContext
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}

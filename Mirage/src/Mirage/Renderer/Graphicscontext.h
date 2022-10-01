#pragma once

namespace Mirage
{
    class GraphicsContext
    {
    public:
        static Scope<GraphicsContext> Create(void* window);
        virtual ~GraphicsContext() = default;
        
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}

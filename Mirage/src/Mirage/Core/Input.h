#pragma once

#include "Math.h"
#include "Mirage/Core/Core.h"

namespace Mirage
{
    class Input
    {
    public:        
        static bool IsKeyPressed(int keyCode);
        static bool IsKeyReleased(int keyCode);

        static bool IsMouseButtonPressed(int button);
        static bool IsMouseButtonReleased(int button);
        
        static Vec2 GetMousePosition();
        
        static float GetMouseX();
        static float GetMouseY();
    };
}

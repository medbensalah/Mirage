#pragma once

#include "Math.h"
#include "Mirage/Core/KeyCodes.h"
#include "Mirage/Core/MouseCodes.h"

namespace Mirage
{
    class Input
    {
    public:        
        static bool IsKeyPressed(KeyCode keyCode);
        static bool IsKeyReleased(KeyCode keyCode);

        static bool IsMouseButtonPressed(MouseCode button);
        static bool IsMouseButtonReleased(MouseCode button);
        
        static Vec2 GetMousePosition();
        
        static float GetMouseX();
        static float GetMouseY();
    };
}

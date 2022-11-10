#pragma once

#include "Mirage/Core/KeyCodes.h"
#include "Mirage/Core/MouseCodes.h"
#include "Mirage/Math/glmTypes.h"

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

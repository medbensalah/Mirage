#pragma once

#include "Mirage/Renderer/Camera.h"

namespace Mirage
{
    
    struct CameraComponent
    {
        Camera Camera;
        bool IsMain = true;
        
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(const Mat4& projection)
            : Camera(projection)
        {
        }
    };
}

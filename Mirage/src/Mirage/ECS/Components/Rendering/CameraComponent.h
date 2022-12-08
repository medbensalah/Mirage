#pragma once

#include "Mirage/ECS/SceneCamera.h"

namespace Mirage
{
    struct CameraComponent
    {
        SceneCamera Camera;
        bool IsMain = true;
        bool FixedAspectRatio = false;

        //TODO : Add clear color
        
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };
}

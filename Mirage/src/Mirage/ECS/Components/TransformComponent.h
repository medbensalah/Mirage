#pragma once
#include "Mirage/Core/Math.h"

namespace Mirage
{
    struct TransformComponent
    {
        Vec3 Position {0.0f, 0.0f, 0.0f};
        Vec3 Rotation {0.0f, 0.0f, 0.0f};
        Vec3 Scale {1.0f, 1.0f, 1.0f};
        

        TransformComponent() = default;
        TransformComponent(const Vec3& position)
            : Position(position)
        {
        }

        Mat4 GetTransform() const
        {
            Mat4 rotation = glm::rotate(Mat4(1.0f), Radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f)) *
                            glm::rotate(Mat4(1.0f), Radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f)) *
                            glm::rotate(Mat4(1.0f), Radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
            return  glm::translate(Mat4(1.0f), Position) *
                    rotation *
                    glm::scale(Mat4(1.0f), Scale);
        }
    };
}

#pragma once
#include "Mirage/Core/Math.h"

namespace Mirage
{
    class Camera
    {
    public:
        Camera() = default;
        Camera(const Mat4& projection)
            : m_Projection(projection)
        {
        }
        virtual ~Camera() = default;

        const Mat4& GetProjection () const { return m_Projection; }
        
    protected:
        Mat4 m_Projection = Mat4(1.0f);
    };
}

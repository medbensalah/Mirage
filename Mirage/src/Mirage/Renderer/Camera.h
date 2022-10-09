#pragma once
#include "Mirage/Core/Math.h"

namespace Mirage
{
    class Camera
    {
    public:
        Camera(const Mat4& projection)
            : m_Projection(projection)
        {
        }

        const Mat4& GetProjection () const { return m_Projection; }
        
    private:
        Mat4 m_Projection;
    };
}

#pragma once
#include "Mirage/Math/glmTypes.h"

namespace Mirage
{
    class Camera
    {
    public:
        enum ProjectionType { Perspective = 0, Orthographic = 1 };
    public:
        Camera() = default;
        Camera(const Mat4& projection)
            : m_Projection(projection)
        {
        }
        virtual ~Camera() = default;

        const Mat4& GetProjection () const { return m_Projection; }

    	const Vec4& GetClearColor() const { return m_ClearColor; }
    	void SetClearColor(const Vec4& color) { m_ClearColor = color; }
        
    protected:
    	Vec4 m_ClearColor = { 0.3f, 0.3f, 0.3f, 1.0f };
        Mat4 m_Projection = Mat4(1.0f);
    };
}

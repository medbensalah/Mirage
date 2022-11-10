#include "MrgPch.h"

#include "Math.h"
#include "glm/gtx/matrix_decompose.hpp"

namespace Mirage::Math
{
    bool DecomposeTransform(const Mat4& transform, Vec3& position, Vec3& rotation, Vec3& scale)
    {
        // From glm::decompose
using namespace glm;
        using T = float;
        
        Mat4 localMatrix(transform);
        //Normalize matrix
        if (epsilonEqual(localMatrix[3][3], static_cast<T>(0), epsilon<T>()))
            return false;

        //Isolate perspective
        if (epsilonNotEqual(localMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(localMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(localMatrix[2][3], static_cast<T>(0), epsilon<T>()))
        {
            //clear the perspective partition
            localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<T>(0);
            localMatrix[3][3] = static_cast<T>(1);
        }

        //Translation
        position = Vec3(localMatrix[3]);
        localMatrix[3] = Vec4(0, 0, 0, localMatrix[3].w);

        Vec3 Row[3], Pdum3;

        //Scale and shear
        for (length_t i = 0; i < 3; ++i)
            for (length_t j = 0; j < 3; ++j)
                Row[i][j] = localMatrix[i][j];

        //Compute X scale factor and normalize first row
        scale.x = length(Row[0]);
        Row[0] = detail::scale(Row[0], static_cast<T>(1));
        scale.y = length(Row[1]);
        Row[1] = detail::scale(Row[1], static_cast<T>(1));
        scale.z = length(Row[2]);
        Row[2] = detail::scale(Row[2], static_cast<T>(1));

#if 1
        //At this point, the matrix (in rows[]) is orthonormal.
        //Check for a coordinate system flip.  If the determinant
        //is -1, then negate the matrix and the scaling factors.
        Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
        if (dot(Row[0], Pdum3) < 0)
        {
            for (length_t i = 0; i < 3; i++)
            {
                scale[i] *= static_cast<T>(-1);
                Row[i] *= static_cast<T>(-1);
            }
        }
#endif
        //Rotation
        rotation.y = asin(-Row[0][2]);
        if (cos(rotation.y) != 0)
        {
            rotation.x = atan2(Row[1][2], Row[2][2]);
            rotation.z = atan2(Row[0][1], Row[0][0]);
        }
        else
        {
            rotation.x = atan2(-Row[2][0], Row[1][1]);
            rotation.z = 0;
        }

        return true;        
    }
}

#pragma once
#include <string>

namespace Mirage
{
    struct MeshComponent
    {
        enum class Primitive : uint8_t
        {
            Cube = 0,
            Sphere = 1
        };

        Primitive PrimitiveType = Primitive::Cube;
        std::string SourcePath;
        bool OptimizeOnLoad = true;
    };
}

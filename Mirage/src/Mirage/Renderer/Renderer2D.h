#pragma once

#include "OrthographicCamera.h"

#include "Primitives.h"

namespace Mirage
{
    namespace Renderer2D
    {
        void Init();
        void Shutdown();

        void BeginScene(const OrthographicCamera& camera);
        void EndScene();

        namespace Draw
        {
            void Quad(Primitives::Quad quad);
        }
    };
}

#pragma once

#include "Mirage/Renderer/OrthographicCamera.h"

#include "Mirage/Renderer/Primitives.h"

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

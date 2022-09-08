#pragma once

#include "Core.h"

namespace Mirage
{
    MIRAGE_CLASS Input
    {
    public:
        static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressed_Impl(keyCode); }
        static bool IsKeyReleased(int keyCode) { return s_Instance->IsKeyReleased_Impl(keyCode); }
        
        static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressed_Impl(button); }
        static bool IsMouseButtonReleased(int button) { return s_Instance->IsMouseButtonReleased_Impl(button); }

        static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePosition_Impl(); }
        static float GetMouseX() { return s_Instance->GetMouseX_Impl(); }
        static float GetMouseY() { return s_Instance->GetMouseY_Impl(); }

    protected:
        virtual bool IsKeyPressed_Impl(int keyCode) = 0;
        virtual bool IsKeyReleased_Impl(int keyCode) = 0;
        
        virtual bool IsMouseButtonPressed_Impl(int button) = 0;
        virtual bool IsMouseButtonReleased_Impl(int button) = 0;

        virtual std::pair<float, float> GetMousePosition_Impl() = 0;
        virtual float GetMouseX_Impl() = 0;
        virtual float GetMouseY_Impl() = 0;

    private:
        static Input* s_Instance;
    };
}

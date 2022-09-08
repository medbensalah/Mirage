#pragma once

#include "Mirage/Input.h"

namespace Mirage
{
    class WindowsInput : public Input
    {
    protected:
        virtual bool IsKeyPressed_Impl(int keyCode) override;
        virtual bool IsKeyReleased_Impl(int keyCode) override;
        virtual bool IsMouseButtonPressed_Impl(int button) override;
        virtual bool IsMouseButtonReleased_Impl(int button) override;
        virtual std::pair<float, float> GetMousePosition_Impl() override;
        virtual float GetMouseX_Impl() override;
        virtual float GetMouseY_Impl() override;
    };
}

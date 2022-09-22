#include "MrgPch.h"

#include "Platform/Windows/WindowsInput.h"

#include <GLFW/glfw3.h>

#include "Mirage/Core/Application.h"
#include "Mirage/Core/KeyCodes.h"
#include "Mirage/Core/MouseButtonCodes.h"

namespace Mirage
{
    Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

    bool WindowsInput::IsKeyPressed_Impl(int keyCode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, MRG_KEY_TO_GLFW_KEY(keyCode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsKeyReleased_Impl(int keyCode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, MRG_KEY_TO_GLFW_KEY(keyCode));
        return state == GLFW_RELEASE;
    }

    bool WindowsInput::IsMouseButtonPressed_Impl(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, MRG_BTN_TO_GLFW_BTN(button));
        return state == GLFW_PRESS;
    }

    bool WindowsInput::IsMouseButtonReleased_Impl(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, MRG_BTN_TO_GLFW_BTN(button));
        return state == GLFW_RELEASE;
    }

    std::pair<float, float> WindowsInput::GetMousePosition_Impl()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return {(float)xPos, (float)yPos};
    }

    float WindowsInput::GetMouseX_Impl()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return (float)xPos;
    }

    float WindowsInput::GetMouseY_Impl()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return (float)yPos;
    }
}

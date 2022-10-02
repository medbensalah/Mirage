#include "MrgPch.h"

#include "Mirage/Core/Input.h"

#include <GLFW/glfw3.h>

#include "Mirage/Core/Application.h"
#include "Mirage/Core/KeyCodes.h"
#include "Mirage/Core/MouseButtonCodes.h"

namespace Mirage
{
    bool Input::IsKeyPressed(int keyCode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, MRG_KEY_TO_GLFW_KEY(keyCode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsKeyReleased(int keyCode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, MRG_KEY_TO_GLFW_KEY(keyCode));
        return state == GLFW_RELEASE;
    }

    bool Input::IsMouseButtonPressed(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, MRG_BTN_TO_GLFW_BTN(button));
        return state == GLFW_PRESS;
    }

    bool Input::IsMouseButtonReleased(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, MRG_BTN_TO_GLFW_BTN(button));
        return state == GLFW_RELEASE;
    }

    Vec2 Input::GetMousePosition()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return {xPos, yPos};
    }

    float Input::GetMouseX()
    {
        return GetMousePosition().x;
    }

    float Input::GetMouseY()
    {
        return GetMousePosition().y;
    }
}

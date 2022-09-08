#pragma once

namespace Mirage
{
    enum ButtonCodes
    {
        MRG_MouseButton_1 = 0,
        MRG_MouseButton_2 = 1,
        MRG_MouseButton_3 = 2,
        MRG_MouseButton_4 = 3,
        MRG_MouseButton_5 = 4,
        MRG_MouseButton_6 = 5,
        MRG_MouseButton_7 = 6,
        MRG_MouseButton_8 = 7,

        MRG_MouseButton_Last = 7,

        MRG_MouseButton_Left = 0,
        MRG_MouseButton_Right = 1,
        MRG_MouseButton_Middle = 2,
    };

    static int GlfwBtnToMrgBtn(int keyCode)
    {
        switch (keyCode)
        {
        case GLFW_MOUSE_BUTTON_LEFT:            return MRG_MouseButton_Left;
        case GLFW_MOUSE_BUTTON_RIGHT:           return MRG_MouseButton_Right;
        case GLFW_MOUSE_BUTTON_MIDDLE:          return MRG_MouseButton_Middle;
        case GLFW_MOUSE_BUTTON_4:               return MRG_MouseButton_4;
        case GLFW_MOUSE_BUTTON_5:               return MRG_MouseButton_5;
        case GLFW_MOUSE_BUTTON_6:               return MRG_MouseButton_6;
        case GLFW_MOUSE_BUTTON_7:               return MRG_MouseButton_7;
        case GLFW_MOUSE_BUTTON_8:               return MRG_MouseButton_8;
        default:                                return MRG_MouseButton_Last;
        }
    }

    static int MrgBtnToGlfwBtn(int keyCode)
    {
        switch (keyCode)
        {
        case MRG_MouseButton_Left:              return GLFW_MOUSE_BUTTON_LEFT;
        case MRG_MouseButton_Right:             return GLFW_MOUSE_BUTTON_RIGHT;
        case MRG_MouseButton_Middle:            return GLFW_MOUSE_BUTTON_MIDDLE;
        case MRG_MouseButton_4:                 return GLFW_MOUSE_BUTTON_4;
        case MRG_MouseButton_5:                 return GLFW_MOUSE_BUTTON_5;
        case MRG_MouseButton_6:                 return GLFW_MOUSE_BUTTON_6;
        case MRG_MouseButton_7:                 return GLFW_MOUSE_BUTTON_7;
        case MRG_MouseButton_8:                 return GLFW_MOUSE_BUTTON_8;
        default:                                return GLFW_MOUSE_BUTTON_LAST;
        }
    }


#define GLFW_BTN_TO_MRG_BTN(x) GlfwBtnToMrgBtn(x)
#define MRG_BTN_TO_GLFW_BTN(x) MrgBtnToGlfwBtn(x)
}

#pragma once
#include "../../vendor/GLFW/include/GLFW/glfw3.h"

namespace Mirage
{
#if OLD_KEYS
    enum KeyCodes
    {
        MRG_Key_Unknown             = -1,

        /* Function keys */
        MRG_Key_KeyPad_0            = 0,
        MRG_Key_KeyPad_1            = 1,
        MRG_Key_KeyPad_2            = 2,
        MRG_Key_KeyPad_3            = 3,
        MRG_Key_KeyPad_4            = 4,
        MRG_Key_KeyPad_5            = 5,
        MRG_Key_KeyPad_6            = 6,
        MRG_Key_KeyPad_7            = 7,
        MRG_Key_KeyPad_8            = 8,
        MRG_Key_KeyPad_9            = 9,
    
        MRG_Key_KeyPad_Decimal      = 10,
        MRG_Key_KeyPad_Divide       = 11,
        MRG_Key_KeyPad_Multiply     = 12,
        MRG_Key_KeyPad_Subtract     = 13,
        MRG_Key_KeyPad_Add          = 14,
        MRG_Key_KeyPad_Equal        = 15,
    
        MRG_Key_KeyPad_Enter        = 16,
        MRG_Key_Enter               = 17,
        
        MRG_Key_Backspace           = 18,
        MRG_Key_Tab                 = 19,
        MRG_Key_Right               = 20,
        MRG_Key_Left                = 21,
        MRG_Key_Down                = 22,
        MRG_Key_Up                  = 23,
        MRG_Key_PageUp              = 24,
        MRG_Key_PageDown            = 25,
        MRG_Key_Home                = 26,
        MRG_Key_End                 = 27,
        MRG_Key_CapsLock            = 28,
        MRG_Key_ScrollLock          = 29,
        MRG_Key_NumLock             = 30,
        MRG_Key_Pause               = 31,
        MRG_Key_Insert              = 32,
        MRG_Key_Delete              = 33,
        MRG_Key_PrintScreen         = 34,
        
        MRG_Key_Escape              = 256,
        
        MRG_Key_F1                  = 35,
        MRG_Key_F2                  = 36,
        MRG_Key_F3                  = 37,
        MRG_Key_F4                  = 38,
        MRG_Key_F5                  = 39,
        MRG_Key_F6                  = 40,
        MRG_Key_F7                  = 41,
        MRG_Key_F8                  = 42,
        MRG_Key_F9                  = 43,
        MRG_Key_F10                 = 44,
        MRG_Key_F11                 = 45,
        MRG_Key_F12                 = 46,
        MRG_Key_F13                 = 47,
        MRG_Key_F14                 = 48,
        MRG_Key_F15                 = 49,
        MRG_Key_F16                 = 50,
        MRG_Key_F17                 = 51,
        MRG_Key_F18                 = 52,
        MRG_Key_F19                 = 53,
        MRG_Key_F20                 = 54,
        MRG_Key_F21                 = 55,
        MRG_Key_F22                 = 56,
        MRG_Key_F23                 = 57,
        MRG_Key_F24                 = 58,
        MRG_Key_F25                 = 59,
        MRG_Key_Menu                = 60,

        /* Printable keys */
        MRG_Key_0                   = 61,
        MRG_Key_1                   = 62,
        MRG_Key_2                   = 63,
        MRG_Key_3                   = 64,
        MRG_Key_4                   = 65,
        MRG_Key_5                   = 66,
        MRG_Key_6                   = 67,
        MRG_Key_7                   = 68,
        MRG_Key_8                   = 69,
        MRG_Key_9                   = 70,
        
        MRG_Key_A                   = 71,
        MRG_Key_B                   = 72,
        MRG_Key_C                   = 73,
        MRG_Key_D                   = 74,
        MRG_Key_E                   = 75,
        MRG_Key_F                   = 76,
        MRG_Key_G                   = 77,
        MRG_Key_H                   = 78,
        MRG_Key_I                   = 79,
        MRG_Key_J                   = 80,
        MRG_Key_K                   = 81,
        MRG_Key_L                   = 82,
        MRG_Key_M                   = 83,
        MRG_Key_N                   = 84,
        MRG_Key_O                   = 85,
        MRG_Key_P                   = 86,
        MRG_Key_Q                   = 87,
        MRG_Key_R                   = 88,
        MRG_Key_S                   = 89,
        MRG_Key_T                   = 90,
        MRG_Key_U                   = 91,
        MRG_Key_V                   = 92,
        MRG_Key_W                   = 93,
        MRG_Key_X                   = 94,
        MRG_Key_Y                   = 95,
        MRG_Key_Z                   = 96,
        MRG_Key_Comma               = 97,
        MRG_Key_Period              = 98,
        MRG_Key_LeftBracket         = 99,
        MRG_Key_RightBracket        = 100,
        MRG_Key_Semicolon           = 101, 
        MRG_Key_Backslash           = 102,
        MRG_Key_GraveAccent         = 103,
        MRG_Key_Apostrophe          = 104,
        MRG_Key_Equal               = 105,
        MRG_Key_Minus               = 106,
        MRG_Key_Slash               = 107,
        MRG_Key_Space               = 108,
        MRG_Key_World1              = 109,
        MRG_Key_World2              = 110,
        
        /* mod keys */
        MRG_Key_LeftShift           = 111,
        MRG_Key_RightShift          = 112,
        MRG_Key_LeftControl         = 113,
        MRG_Key_RightControl        = 114,
        MRG_Key_LeftAlt             = 115,
        MRG_Key_RightAlt            = 116,
        MRG_Key_LeftSuper           = 117,
        MRG_Key_RightSuper          = 118,

        /* Extra */
        MRG_KEY_Last                = 119
    };

    static int GlfwKeyToMrgKey(int keyCode)
    {
        switch (keyCode)
        {
            case GLFW_KEY_SPACE:                return MRG_Key_Space;
            case GLFW_KEY_APOSTROPHE:           return MRG_Key_Apostrophe;
            case GLFW_KEY_COMMA:                return MRG_Key_Comma;
            case GLFW_KEY_MINUS:                return MRG_Key_Minus;
            case GLFW_KEY_PERIOD:               return MRG_Key_Period;
            case GLFW_KEY_SLASH:                return MRG_Key_Slash;
            case GLFW_KEY_0:                    return MRG_Key_0;
            case GLFW_KEY_1:                    return MRG_Key_1;
            case GLFW_KEY_2:                    return MRG_Key_2;
            case GLFW_KEY_3:                    return MRG_Key_3;
            case GLFW_KEY_4:                    return MRG_Key_4;
            case GLFW_KEY_5:                    return MRG_Key_5;
            case GLFW_KEY_6:                    return MRG_Key_6;
            case GLFW_KEY_7:                    return MRG_Key_7;
            case GLFW_KEY_8:                    return MRG_Key_8;
            case GLFW_KEY_9:                    return MRG_Key_9;
            case GLFW_KEY_SEMICOLON:            return MRG_Key_Semicolon;
            case GLFW_KEY_EQUAL:                return MRG_Key_Equal;
            case GLFW_KEY_A:                    return MRG_Key_A;
            case GLFW_KEY_B:                    return MRG_Key_B;
            case GLFW_KEY_C:                    return MRG_Key_C;
            case GLFW_KEY_D:                    return MRG_Key_D;
            case GLFW_KEY_E:                    return MRG_Key_E;
            case GLFW_KEY_F:                    return MRG_Key_F;
            case GLFW_KEY_G:                    return MRG_Key_G;
            case GLFW_KEY_H:                    return MRG_Key_H;
            case GLFW_KEY_I:                    return MRG_Key_I;
            case GLFW_KEY_J:                    return MRG_Key_J;
            case GLFW_KEY_K:                    return MRG_Key_K;
            case GLFW_KEY_L:                    return MRG_Key_L;
            case GLFW_KEY_M:                    return MRG_Key_M;
            case GLFW_KEY_N:                    return MRG_Key_N;
            case GLFW_KEY_O:                    return MRG_Key_O;
            case GLFW_KEY_P:                    return MRG_Key_P;
            case GLFW_KEY_Q:                    return MRG_Key_Q;
            case GLFW_KEY_R:                    return MRG_Key_R;
            case GLFW_KEY_S:                    return MRG_Key_S;
            case GLFW_KEY_T:                    return MRG_Key_T;
            case GLFW_KEY_U:                    return MRG_Key_U;
            case GLFW_KEY_V:                    return MRG_Key_V;
            case GLFW_KEY_W:                    return MRG_Key_W;
            case GLFW_KEY_X:                    return MRG_Key_X;
            case GLFW_KEY_Y:                    return MRG_Key_Y;
            case GLFW_KEY_Z:                    return MRG_Key_Z;
            case GLFW_KEY_LEFT_BRACKET:         return MRG_Key_LeftBracket;
            case GLFW_KEY_BACKSLASH:            return MRG_Key_Backslash;
            case GLFW_KEY_RIGHT_BRACKET:        return MRG_Key_RightBracket;
            case GLFW_KEY_GRAVE_ACCENT:         return MRG_Key_GraveAccent;
            case GLFW_KEY_WORLD_1:              return MRG_Key_World1;
            case GLFW_KEY_WORLD_2:              return MRG_Key_World2;
            case GLFW_KEY_ESCAPE:               return MRG_Key_Escape;
            case GLFW_KEY_ENTER:                return MRG_Key_Enter;
            case GLFW_KEY_TAB:                  return MRG_Key_Tab;
            case GLFW_KEY_BACKSPACE:            return MRG_Key_Backspace;
            case GLFW_KEY_INSERT:               return MRG_Key_Insert;
            case GLFW_KEY_DELETE:               return MRG_Key_Delete;
            case GLFW_KEY_RIGHT:                return MRG_Key_Right;
            case GLFW_KEY_LEFT:                 return MRG_Key_Left;
            case GLFW_KEY_DOWN:                 return MRG_Key_Down;
            case GLFW_KEY_UP:                   return MRG_Key_Up;
            case GLFW_KEY_PAGE_UP:              return MRG_Key_PageUp;
            case GLFW_KEY_PAGE_DOWN:            return MRG_Key_PageDown;
            case GLFW_KEY_HOME:                 return MRG_Key_Home;
            case GLFW_KEY_END:                  return MRG_Key_End;
            case GLFW_KEY_CAPS_LOCK:            return MRG_Key_CapsLock;
            case GLFW_KEY_SCROLL_LOCK:          return MRG_Key_ScrollLock;
            case GLFW_KEY_NUM_LOCK:             return MRG_Key_NumLock;
            case GLFW_KEY_PRINT_SCREEN:         return MRG_Key_PrintScreen;
            case GLFW_KEY_PAUSE:                return MRG_Key_Pause;
            case GLFW_KEY_F1:                   return MRG_Key_F1;
            case GLFW_KEY_F2:                   return MRG_Key_F2;
            case GLFW_KEY_F3:                   return MRG_Key_F3;
            case GLFW_KEY_F4:                   return MRG_Key_F4;
            case GLFW_KEY_F5:                   return MRG_Key_F5;
            case GLFW_KEY_F6:                   return MRG_Key_F6;
            case GLFW_KEY_F7:                   return MRG_Key_F7;
            case GLFW_KEY_F8:                   return MRG_Key_F8;
            case GLFW_KEY_F9:                   return MRG_Key_F9;
            case GLFW_KEY_F10:                  return MRG_Key_F10;
            case GLFW_KEY_F11:                  return MRG_Key_F11;
            case GLFW_KEY_F12:                  return MRG_Key_F12;
            case GLFW_KEY_F13:                  return MRG_Key_F13;
            case GLFW_KEY_F14:                  return MRG_Key_F14;
            case GLFW_KEY_F15:                  return MRG_Key_F15;
            case GLFW_KEY_F16:                  return MRG_Key_F16;
            case GLFW_KEY_F17:                  return MRG_Key_F17;
            case GLFW_KEY_F18:                  return MRG_Key_F18;
            case GLFW_KEY_F19:                  return MRG_Key_F19;
            case GLFW_KEY_F20:                  return MRG_Key_F20;
            case GLFW_KEY_F21:                  return MRG_Key_F21;
            case GLFW_KEY_F22:                  return MRG_Key_F22;
            case GLFW_KEY_F23:                  return MRG_Key_F23;
            case GLFW_KEY_F24:                  return MRG_Key_F24;
            case GLFW_KEY_F25:                  return MRG_Key_F25;
            case GLFW_KEY_KP_0:                 return MRG_Key_KeyPad_0;
            case GLFW_KEY_KP_1:                 return MRG_Key_KeyPad_1;
            case GLFW_KEY_KP_2:                 return MRG_Key_KeyPad_2;
            case GLFW_KEY_KP_3:                 return MRG_Key_KeyPad_3;
            case GLFW_KEY_KP_4:                 return MRG_Key_KeyPad_4;
            case GLFW_KEY_KP_5:                 return MRG_Key_KeyPad_5;
            case GLFW_KEY_KP_6:                 return MRG_Key_KeyPad_6;
            case GLFW_KEY_KP_7:                 return MRG_Key_KeyPad_7;
            case GLFW_KEY_KP_8:                 return MRG_Key_KeyPad_8;
            case GLFW_KEY_KP_9:                 return MRG_Key_KeyPad_9;
            case GLFW_KEY_KP_DECIMAL:           return MRG_Key_KeyPad_Decimal;
            case GLFW_KEY_KP_DIVIDE:            return MRG_Key_KeyPad_Divide;
            case GLFW_KEY_KP_MULTIPLY:          return MRG_Key_KeyPad_Multiply;
            case GLFW_KEY_KP_SUBTRACT:          return MRG_Key_KeyPad_Subtract;
            case GLFW_KEY_KP_ADD:               return MRG_Key_KeyPad_Add;
            case GLFW_KEY_KP_ENTER:             return MRG_Key_KeyPad_Enter;
            case GLFW_KEY_KP_EQUAL:             return MRG_Key_KeyPad_Equal;
            case GLFW_KEY_LEFT_SHIFT:           return MRG_Key_LeftShift;
            case GLFW_KEY_LEFT_CONTROL:         return MRG_Key_LeftControl;
            case GLFW_KEY_LEFT_ALT:             return MRG_Key_LeftAlt;
            case GLFW_KEY_LEFT_SUPER:           return MRG_Key_LeftSuper;
            case GLFW_KEY_RIGHT_SHIFT:          return MRG_Key_RightShift;
            case GLFW_KEY_RIGHT_CONTROL:        return MRG_Key_RightControl;
            case GLFW_KEY_RIGHT_ALT:            return MRG_Key_RightAlt;
            case GLFW_KEY_RIGHT_SUPER:          return MRG_Key_RightSuper;
            case GLFW_KEY_MENU:                 return MRG_Key_Menu;
            default:                            return MRG_Key_Unknown;
        }
    }

    static int MrgKeyToGlfwKey(int keyCode)
    {
        switch (keyCode)
        {
            case MRG_Key_Space:                 return GLFW_KEY_SPACE;
            case MRG_Key_Apostrophe:            return GLFW_KEY_APOSTROPHE;
            case MRG_Key_Comma:                 return GLFW_KEY_COMMA;
            case MRG_Key_Minus:                 return GLFW_KEY_MINUS;
            case MRG_Key_Period:                return GLFW_KEY_PERIOD;
            case MRG_Key_Slash:                 return GLFW_KEY_SLASH;
            case MRG_Key_0:                     return GLFW_KEY_0;
            case MRG_Key_1:                     return GLFW_KEY_1;
            case MRG_Key_2:                     return GLFW_KEY_2;
            case MRG_Key_3:                     return GLFW_KEY_3;
            case MRG_Key_4:                     return GLFW_KEY_4;
            case MRG_Key_5:                     return GLFW_KEY_5;
            case MRG_Key_6:                     return GLFW_KEY_6;
            case MRG_Key_7:                     return GLFW_KEY_7;
            case MRG_Key_8:                     return GLFW_KEY_8;
            case MRG_Key_9:                     return GLFW_KEY_9;
            case MRG_Key_Semicolon:             return GLFW_KEY_SEMICOLON;
            case MRG_Key_Equal:                 return GLFW_KEY_EQUAL;
            case MRG_Key_A:                     return GLFW_KEY_A;
            case MRG_Key_B:                     return GLFW_KEY_B;
            case MRG_Key_C:                     return GLFW_KEY_C;
            case MRG_Key_D:                     return GLFW_KEY_D;
            case MRG_Key_E:                     return GLFW_KEY_E;
            case MRG_Key_F:                     return GLFW_KEY_F;
            case MRG_Key_G:                     return GLFW_KEY_G;
            case MRG_Key_H:                     return GLFW_KEY_H;
            case MRG_Key_I:                     return GLFW_KEY_I;
            case MRG_Key_J:                     return GLFW_KEY_J;
            case MRG_Key_K:                     return GLFW_KEY_K;
            case MRG_Key_L:                     return GLFW_KEY_L;
            case MRG_Key_M:                     return GLFW_KEY_M;
            case MRG_Key_N:                     return GLFW_KEY_N;
            case MRG_Key_O:                     return GLFW_KEY_O;
            case MRG_Key_P:                     return GLFW_KEY_P;
            case MRG_Key_Q:                     return GLFW_KEY_Q;
            case MRG_Key_R:                     return GLFW_KEY_R;
            case MRG_Key_S:                     return GLFW_KEY_S;
            case MRG_Key_T:                     return GLFW_KEY_T;
            case MRG_Key_U:                     return GLFW_KEY_U;
            case MRG_Key_V:                     return GLFW_KEY_V;
            case MRG_Key_W:                     return GLFW_KEY_W;
            case MRG_Key_X:                     return GLFW_KEY_X;
            case MRG_Key_Y:                     return GLFW_KEY_Y;
            case MRG_Key_Z:                     return GLFW_KEY_Z;
            case MRG_Key_LeftBracket:           return GLFW_KEY_LEFT_BRACKET;
            case MRG_Key_Backslash:             return GLFW_KEY_BACKSLASH;
            case MRG_Key_RightBracket:          return GLFW_KEY_RIGHT_BRACKET;
            case MRG_Key_GraveAccent:           return GLFW_KEY_GRAVE_ACCENT;
            case MRG_Key_World1:                return GLFW_KEY_WORLD_1;
            case MRG_Key_World2:                return GLFW_KEY_WORLD_2;
            case MRG_Key_Escape:                return GLFW_KEY_ESCAPE;
            case MRG_Key_Enter:                 return GLFW_KEY_ENTER;
            case MRG_Key_Tab:                   return GLFW_KEY_TAB;
            case MRG_Key_Backspace:             return GLFW_KEY_BACKSPACE;
            case MRG_Key_Insert:                return GLFW_KEY_INSERT;
            case MRG_Key_Delete:                return GLFW_KEY_DELETE;
            case MRG_Key_Right:                 return GLFW_KEY_RIGHT;
            case MRG_Key_Left:                  return GLFW_KEY_LEFT;
            case MRG_Key_Down:                  return GLFW_KEY_DOWN;
            case MRG_Key_Up:                    return GLFW_KEY_UP;
            case MRG_Key_PageUp:                return GLFW_KEY_PAGE_UP;
            case MRG_Key_PageDown:              return GLFW_KEY_PAGE_DOWN;
            case MRG_Key_Home:                  return GLFW_KEY_HOME;
            case MRG_Key_End:                   return GLFW_KEY_END;
            case MRG_Key_CapsLock:              return GLFW_KEY_CAPS_LOCK;
            case MRG_Key_ScrollLock:            return GLFW_KEY_SCROLL_LOCK;
            case MRG_Key_NumLock:               return GLFW_KEY_NUM_LOCK;
            case MRG_Key_PrintScreen:           return GLFW_KEY_PRINT_SCREEN;
            case MRG_Key_Pause:                 return GLFW_KEY_PAUSE;
            case MRG_Key_F1:                    return GLFW_KEY_F1;
            case MRG_Key_F2:                    return GLFW_KEY_F2;
            case MRG_Key_F3:                    return GLFW_KEY_F3;
            case MRG_Key_F4:                    return GLFW_KEY_F4;
            case MRG_Key_F5:                    return GLFW_KEY_F5;
            case MRG_Key_F6:                    return GLFW_KEY_F6;
            case MRG_Key_F7:                    return GLFW_KEY_F7;
            case MRG_Key_F8:                    return GLFW_KEY_F8;
            case MRG_Key_F9:                    return GLFW_KEY_F9;
            case MRG_Key_F10:                   return GLFW_KEY_F10;
            case MRG_Key_F11:                   return GLFW_KEY_F11;
            case MRG_Key_F12:                   return GLFW_KEY_F12;
            case MRG_Key_F13:                   return GLFW_KEY_F13;
            case MRG_Key_F14:                   return GLFW_KEY_F14;
            case MRG_Key_F15:                   return GLFW_KEY_F15;
            case MRG_Key_F16:                   return GLFW_KEY_F16;
            case MRG_Key_F17:                   return GLFW_KEY_F17;
            case MRG_Key_F18:                   return GLFW_KEY_F18;
            case MRG_Key_F19:                   return GLFW_KEY_F19;
            case MRG_Key_F20:                   return GLFW_KEY_F20;
            case MRG_Key_F21:                   return GLFW_KEY_F21;
            case MRG_Key_F22:                   return GLFW_KEY_F22;
            case MRG_Key_F23:                   return GLFW_KEY_F23;
            case MRG_Key_F24:                   return GLFW_KEY_F24;
            case MRG_Key_F25:                   return GLFW_KEY_F25;
            case MRG_Key_KeyPad_0:              return GLFW_KEY_KP_0;
            case MRG_Key_KeyPad_1:              return GLFW_KEY_KP_1;
            case MRG_Key_KeyPad_2:              return GLFW_KEY_KP_2;
            case MRG_Key_KeyPad_3:              return GLFW_KEY_KP_3;
            case MRG_Key_KeyPad_4:              return GLFW_KEY_KP_4;
            case MRG_Key_KeyPad_5:              return GLFW_KEY_KP_5;
            case MRG_Key_KeyPad_6:              return GLFW_KEY_KP_6;
            case MRG_Key_KeyPad_7:              return GLFW_KEY_KP_7;
            case MRG_Key_KeyPad_8:              return GLFW_KEY_KP_8;
            case MRG_Key_KeyPad_9:              return GLFW_KEY_KP_9;
            case MRG_Key_KeyPad_Decimal:        return GLFW_KEY_KP_DECIMAL;
            case MRG_Key_KeyPad_Divide:         return GLFW_KEY_KP_DIVIDE;
            case MRG_Key_KeyPad_Multiply:       return GLFW_KEY_KP_MULTIPLY;
            case MRG_Key_KeyPad_Subtract:       return GLFW_KEY_KP_SUBTRACT;
            case MRG_Key_KeyPad_Add:            return GLFW_KEY_KP_ADD;
            case MRG_Key_KeyPad_Enter:          return GLFW_KEY_KP_ENTER;
            case MRG_Key_KeyPad_Equal:          return GLFW_KEY_KP_EQUAL;
            case MRG_Key_LeftShift:             return GLFW_KEY_LEFT_SHIFT;
            case MRG_Key_LeftControl:           return GLFW_KEY_LEFT_CONTROL;
            case MRG_Key_LeftAlt:               return GLFW_KEY_LEFT_ALT;
            case MRG_Key_LeftSuper:             return GLFW_KEY_LEFT_SUPER;
            case MRG_Key_RightShift:            return GLFW_KEY_RIGHT_SHIFT;
            case MRG_Key_RightControl:          return GLFW_KEY_RIGHT_CONTROL;
            case MRG_Key_RightAlt:              return GLFW_KEY_RIGHT_ALT;
            case MRG_Key_RightSuper:            return GLFW_KEY_RIGHT_SUPER;
            case MRG_Key_Menu:                  return GLFW_KEY_MENU;
            default:                            return GLFW_KEY_UNKNOWN;
        }
    }


#define GLFW_KEY_TO_MRG_KEY(x) GlfwKeyToMrgKey(x)
#define MRG_KEY_TO_GLFW_KEY(x) MrgKeyToGlfwKey(x)
#endif

// TODO cross platform
    //Temporary fix for windows
    static bool IsQwerty()
    {
        switch (PRIMARYLANGID(LOWORD(GetKeyboardLayout(0))))
        {
        case LANG_FRENCH:
            return false;
            break;
        case LANG_ENGLISH:
            return true;
            break;
        }
    }

    
    using KeyCode = uint16_t;

    namespace Key
    {
        enum : KeyCode
        {
            Unknown = 0,
            Space = 32,
            Apostrophe = 39, /* ' */
            Comma = 44, /* , */
            Minus = 45, /* - */
            Period = 46, /* . */
            Slash = 47, /* / */
            D0 = 48, /* 0 */
            D1 = 49, /* 1 */
            D2 = 50, /* 2 */
            D3 = 51, /* 3 */
            D4 = 52, /* 4 */
            D5 = 53, /* 5 */
            D6 = 54, /* 6 */
            D7 = 55, /* 7 */
            D8 = 56, /* 8 */
            D9 = 57, /* 9 */
            Semicolon = 59, /* ; */
            Equal = 61, /* = */
            A = 65,
            B = 66,
            C = 67,
            D = 68,
            E = 69,
            F = 70,
            G = 71,
            H = 72,
            I = 73,
            J = 74,
            K = 75,
            L = 76,
            M = 77,
            N = 78,
            O = 79,
            P = 80,
            Q = 81,
            R = 82,
            S = 83,
            T = 84,
            U = 85,
            V = 86,
            W = 87,
            X = 88,
            Y = 89,
            Z = 90,
            LeftBracket = 91, /* [ */
            Backslash = 92, /* \ */
            RightBracket = 93, /* ] */
            GraveAccent = 96, /* ` */
            World1 = 161, /* non-US #1 */
            World2 = 162, /* non-US #2 */
            Escape = 256,
            Enter = 257,
            Tab = 258,
            Backspace = 259,
            Insert = 260,
            Delete = 261,
            Right = 262,
            Left = 263,
            Down = 264,
            Up = 265,
            PageUp = 266,
            PageDown = 267,
            Home = 268,
            End = 269,
            CapsLock = 280,
            ScrollLock = 281,
            NumLock = 282,
            PrintScreen = 283,
            Pause = 284,
            F1 = 290,
            F2 = 291,
            F3 = 292,
            F4 = 293,
            F5 = 294,
            F6 = 295,
            F7 = 296,
            F8 = 297,
            F9 = 298,
            F10 = 299,
            F11 = 300,
            F12 = 301,
            F13 = 302,
            F14 = 303,
            F15 = 304,
            F16 = 305,
            F17 = 306,
            F18 = 307,
            F19 = 308,
            F20 = 309,
            F21 = 310,
            F22 = 311,
            F23 = 312,
            F24 = 313,
            F25 = 314,
            KP0 = 320,
            KP1 = 321,
            KP2 = 322,
            KP3 = 323,
            KP4 = 324,
            KP5 = 325,
            KP6 = 326,
            KP7 = 327,
            KP8 = 328,
            KP9 = 329,
            KpDecimal = 330,
            KpDivide = 331,
            KpMultiply = 332,
            KpSubtract = 333,
            KpAdd = 334,
            KpEnter = 335,
            KpEqual = 336,
            LeftShift = 340,
            LeftControl = 341,
            LeftAlt = 342,
            LeftSuper = 343,
            RightShift = 344,
            RightControl = 345,
            RightAlt = 346,
            RightSuper = 347,
            Menu = 348,
            Last = Menu
        };
    }
}

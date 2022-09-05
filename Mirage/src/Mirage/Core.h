#pragma once

#ifdef MRG_PLATFORM_WINDOWS
    #ifdef MRG_BUILD_DLL
        #define MIRAGE_API __declspec(dllexport)
    #else
        #define  MIRAGE_API __declspec(dllimport)
    #endif
#else
#error MIRAGE SUPPORTS WINDOWS ONLY!!!
#endif


#pragma once

#include "Mirage/Core/PlatformDetection.h"

#ifdef MRG_PLATFORM_WINDOWS
    #ifndef NOMINMAX
        // See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
        #define NOMINMAX
    #endif
#endif

#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Mirage/Core/Core.h"

#include "Mirage/Core/Log.h"
#include "Mirage/Debug/Instrumentor.h"

#ifdef MRG_PLATFORM_WINDOWS
    #include<Windows.h>
#endif

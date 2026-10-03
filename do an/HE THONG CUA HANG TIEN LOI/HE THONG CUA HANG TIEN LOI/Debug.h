#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#define DEBUG_MODE 0

#if DEBUG_MODE
    #define LOG_DEBUG(x) std::cout << "[DEBUG] " << x << std::endl
#else
    #define LOG_DEBUG(x)
#endif

#endif
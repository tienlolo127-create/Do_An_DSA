#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <chrono>

#define DEBUG_MODE 1

#if DEBUG_MODE
    #define LOG_DEBUG(x) std::cout << "[DEBUG] " << x << std::endl
#else
    #define LOG_DEBUG(x)
#endif

#define PROFILE_SCOPE(name, code_block) \
    do { \
        auto start = std::chrono::high_resolution_clock::now(); \
        code_block; \
        auto end = std::chrono::high_resolution_clock::now(); \
        std::chrono::duration<double, std::milli> elapsed = end - start; \
        std::cout << "[BENCHMARK] " << name << " mat: " << elapsed.count() << " ms\n"; \
    } while(0)

#endif
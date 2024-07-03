#pragma once

#include "kon/core/Core.hpp"
#include <chrono>
#include <cstdio>
#include <functional>
#include <kon/core/Logging.hpp>

namespace kon
{
    template<typename T>
    void KonCoreTest(const char *name, T expected, const std::function<T(void)> f)
    {
		auto s = std::chrono::high_resolution_clock::now();
        T rec = f();
		auto e = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::microseconds::period>(e - s).count();
		//time /= 1000; // convert to ms
        if(expected == rec) 
            printf("Test passed [%s | us:%f]\n", name, time);
        else
            printf("Test failed [%s | us:%f]\n", name, time);
    };
}

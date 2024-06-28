
#pragma once

namespace kon
{
    // these values will change depending on the system (32 or 64 bit)
    // horray.
    // https://en.cppreference.com/w/cpp/language/types
    using i8 = char;
    using i16 = short int;
    using i32 = long int;
    using i64 = long long int;

	using u8 = unsigned char;
}

#define KN_STRINGIFY(x) #x
#define KN_BIT(x) (1 << x)
#define KN_EXPAND(x) x

#ifdef KN_COMPILE_SHARED
    #define knAPI __declspec(dllexport)
#else
    #define knAPI
#endif

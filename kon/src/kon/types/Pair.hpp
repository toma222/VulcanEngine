
#pragma once

#include "kon/core/Core.hpp"

#include "kon/core/Logging.hpp"

namespace kon
{
    template<typename T, typename U>
    struct knAPI Pair
    {
    public:
        Pair(T f, U s)
            : first(f), second(s) {}
        Pair(T f)
            : first(f), second() {}
        Pair(U s)
            : first(), second(s) {}

        Pair(const Pair<T, U> &pair)
            : first(pair.first), second(pair.second)
        {
            KN_TRACE("Pair copy constructor here");
        }

        Pair() = default;
        ~Pair() = default;

        void operator =(const Pair<T, U> &p)
        {
            first = p.first;
            second = p.second;
        }

        T first;
        U second;
    };
}


#include "Logging.hpp"
#include <stdio.h>

namespace kon
{
    void Logging::Write(int level, const char *c)
    {
        printf(c);
        printf("\n");

        KN_TRACE("asd %i", 1);
    }

    Logging::Logging()
    {
        KN_TRACE("Creating Logging");
    }

    Logging::~Logging()
    {
        KN_TRACE("Destroying Logging");
    }
}

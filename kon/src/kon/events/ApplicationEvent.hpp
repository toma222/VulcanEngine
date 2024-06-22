
#pragma once

#include "Event.hpp"

namespace kon
{
    class EventApplicationExit : public Event
    {
    KN_EVENT(EventApplicationExit, Application)

    public:
        EventApplicationExit() = default;
        ~EventApplicationExit() = default;
    };
}

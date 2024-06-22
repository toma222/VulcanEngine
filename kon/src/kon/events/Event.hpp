
#pragma once

#include "kon/types/String.hpp"

namespace kon
{
    enum class knAPI EventType
    {
        None,
        EventApplicationExit
    };

    enum class knAPI EventClass : i16
    {
        None = 0,
        Application = KN_BIT(0),
        Keyboard = KN_BIT(1)
    };

    #define KN_EVENT_CLASSES 2

    class knAPI Event
    {
    public:
        virtual ~Event() = default;

        virtual const String GetEventName() const = 0;
        virtual const EventType GetEventType() const = 0;
        virtual const i16 GetEventClass() const = 0;
    };

    #define KN_EVENT(name, class) public: \
                           const String GetEventName() const override { return #name; } \
                           const EventType GetEventType() const override { return EventType::name; } \
                           const i16 GetEventClass() const override { return (i16)EventClass::class; }
}

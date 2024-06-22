
#include "Context.hpp"
#include "Logging.hpp"

#include <stdio.h>
#include <cmath>
#include <kon/debug/Debug.hpp>

#define LOG2(X) ((unsigned) (8*sizeof (unsigned long long) - __builtin_clzll((X)) - 1))

namespace kon
{
    Context::Context()
    {
        KN_INSTRUMENT_FUNCTION()
        KN_TRACE("Context Created");

        // Create all the event stuff
        for(int i = 0; i < (KN_EVENT_CLASSES); i++)
        {
            // KN_TRACE("ok");
            // eventRecievingObjects.Add(
            //     Pair<EventClass, ArrayList<Object*>>(static_cast<EventClass>(KN_BIT(i))));
        }   

        // ArrayList<Pair<EventClass, ArrayList<Object*>>> test;
        // eventRecievingObjects.Add(Pair<EventClass, ArrayList<Object*>>(EventClass::Application));
    }

    Context::~Context()
    {
        KN_INSTRUMENT_FUNCTION()
        KN_TRACE("Context Destroyed");
    }

    void Context::AddModule(Object *object)
    {
        KN_INSTRUMENT_FUNCTION()
        modules.Add(object);
    }

    void Context::AddListener(Object *object, i16 classes)
    {
        KN_INSTRUMENT_FUNCTION()
        KN_TRACE("Adding listener");

        for(int i = 0; i < KN_EVENT_CLASSES; i++)
        {
            if(classes & (i16)(KN_BIT(i)))
            {
                eventRecievingObjects[i].Add(object);
            }
        }
    }

    void Context::EmitEvent(Event &event)
    {
        KN_INSTRUMENT_FUNCTION()
        int eventIndex = LOG2(event.GetEventClass());
        int c = eventRecievingObjects[eventIndex].Index();
        for(int i = 0; i < c; i++)
        {
            eventRecievingObjects[eventIndex].Get(i)->OnEvent(event);
        }
    }

    void Context::ClearModules()
    {
        KN_INSTRUMENT_FUNCTION()
        for(int i = 0; i < modules.Index(); i++)
            delete modules.Get(i);
    }
}


#pragma once

#include "kon/types/ArrayList.hpp"
#include "kon/types/Pair.hpp"
#include "Object.hpp"
#include "kon/core/Core.hpp"

namespace kon
{
    // Stores all the subsystems
    class knAPI Context
    {
    public:
        Context();
        ~Context();

        void AddModule(Object* object);
        void AddListener(Object *object, i16 classes);

        void EmitEvent(Event &event);

        void ClearModules();

    private:
        ArrayList<Object*> modules;
        ArrayList<Object*> eventRecievingObjects[KN_EVENT_CLASSES];
    };
}

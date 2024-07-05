
#pragma once

#include "kon/types/ArrayList.hpp"
#include "kon/types/Pair.hpp"
#include "Object.hpp"
#include "kon/core/Core.hpp"
#include <kon/core/System.hpp>

namespace kon
{
    // Stores all the subsystems
    class knAPI Context
    {
    public:
        Context();
        ~Context();

        void AddSystem(System *system);
        void SystemUpdate();

        void AddListener(Object *object, i16 classes);

        void EmitEvent(Event &event);

        void ClearSystems();

    private:
        ArrayList<System*> systems;
        ArrayList<Object*> eventRecievingObjects[KN_EVENT_CLASSES];
    };
}

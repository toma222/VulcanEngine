
#pragma once

#include "kon/types/ArrayList.hpp"
#include "kon/types/Pair.hpp"
#include "Object.hpp"
#include "kon/core/Core.hpp"

#include <kon/types/Map.hpp>
#include <kon/core/System.hpp>
#include <kon/resource/Resource.hpp>

namespace kon
{
    // Stores all the subsystems
    class knAPI Context
    {
    public:
        Context();
        ~Context();

        // Systems
        void AddSystem(System *system);
        void SystemUpdate();
        void ClearSystems();

        // Events
        void AddListener(Object *object, i16 classes);
        void EmitEvent(Event &event);

        // Resources
        // Resource *LoadOrGetResource(Resource **resource, const String &path)
        template<typename R>
        R *LoadOrGetResource(const String &path)
        {
            if(resourceMap.HasDuplicate(path.Hash())) 
                return resourceMap.Get(path.Hash());

            // i am aware just how epic this looks
            // it also might be UB but idk
            R *r = new R(path);
            r->LoadResource(path);
            resourceMap.Enter(r, path.Hash());
        }

        Resource *GetResource(String &path) const;
        Resource *GetResource(u32 hash) const;

        bool HasResource(String &path) const;
        bool HasResource(u32 hash) const;

    private:
        ArrayList<System*> systems;
        ArrayList<Object*> eventRecievingObjects[KN_EVENT_CLASSES];
        HashMap<Resource*, u32> resourceMap;
    };
}

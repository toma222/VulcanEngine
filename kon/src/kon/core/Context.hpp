
#pragma once

#include "kon/types/ArrayList.hpp"
#include "kon/types/Pair.hpp"
#include "Object.hpp"
#include "kon/core/Core.hpp"
#include "kon/types/Path.hpp"

#include <kon/types/Map.hpp>
#include <kon/core/System.hpp>
#include <kon/resource/Resource.hpp>

namespace kon
{
    // Stores all the subsystems
    class knAPI Context
    {
    public:
        Context(Path projectPath);
        ~Context();

        // Systems
        void AddSystem(System *system);
        void SystemUpdate();
        void ClearSystems();

        // Events
        void AddListener(Object *object, i16 classes);
        void EmitEvent(Event &event);

        // assumes that the resource is already loaded
        // it just takes the pointer and owenership over the pointer
        void RegisterResource(Resource *resource, String path);
        void RegisterResource(Resource *resource, u32 hash);

        Resource *GetResource(String path) const;
        Resource *GetResource(u32 hash) const;

        bool HasResource(String &path) const;
        bool HasResource(u32 hash) const;

		Path GetProjectPath() const { return m_projectPath; }

    private:
        ArrayList<System*> systems;
        ArrayList<Object*> eventRecievingObjects[KN_EVENT_CLASSES];
        HashMap<Resource*, u32> resourceMap;
		Path m_projectPath;
    };
}

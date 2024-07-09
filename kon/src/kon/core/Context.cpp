
#include "Context.hpp"
#include "Logging.hpp"

#include <stdio.h>
#include <cmath>
#include <kon/debug/Debug.hpp>

#define LOG2(X) ((unsigned) (8*sizeof (unsigned long long) - __builtin_clzll((X)) - 1))

namespace kon
{
    Context::Context(Path projectPath)
		: m_projectPath(projectPath)
    {
        KN_INSTRUMENT_FUNCTION()
        KN_TRACE("Context Created");
    }

    Context::~Context()
    {
        KN_INSTRUMENT_FUNCTION()

		for(int i = 0; i < resourceArray.Index(); i++)
			delete resourceArray.Get(i);

        KN_TRACE("Context Destroyed");
    }

    void Context::AddSystem(System *system)
    {
        KN_INSTRUMENT_FUNCTION()
        systems.Add(system);
    }

    void Context::SystemUpdate()
    {
        for(int i = 0; i < systems.Index(); i++)
        {
            systems.Get(i)->Update();
        }
    }

    void Context::AddListener(Object *object, i16 classes)
    {
        KN_INSTRUMENT_FUNCTION()
		KN_TRACE("Adding listener [%s]", object->GetTypeInfo()->m_name.c_str());

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

    void Context::ClearSystems()
    {
        KN_INSTRUMENT_FUNCTION()
        KN_TRACE("clear systems");
        for(int i = 0; i < systems.Index(); i++)
            delete systems.GetCopy(i);
    }

    void Context::RegisterResource(Resource *resource, String path)
    {
		String fileName = path.SubString(path.LastIndexOfChar('/')+1, path.GetSize());
		resourceMap.Emplace(resource, fileName.Hash());
		resourceArray.Add(resource);
    }

    void Context::RegisterResource(Resource *resource, u32 path)
    {
		KN_WARN("register resource (resource, hash) might not work");
        // resourceMap.Emplace(resource, path);
    }

    Resource *Context::GetResource(String path) const
    {
        return resourceMap.Get(path.Hash());
    }

    Resource *Context::GetResource(u32 hash) const
    {
        return resourceMap.Get(hash);
    }

    bool Context::HasResource(String &path) const
    {
        return resourceMap.CheckCollision(path.Hash());
    }

    bool Context::HasResource(u32 hash) const
    {
        return resourceMap.CheckCollision(hash);
    }
}

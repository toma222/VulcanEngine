
#pragma once

#include "kon/core/Object.hpp"
#include "kon/types/String.hpp"
#include "kon/types/Allocator.hpp"
#include "kon/core/Core.hpp"
#include <kon/types/ArrayList.hpp>
#include <kon/types/Pair.hpp>

namespace kon
{
    class Context;

    class Resource : public Object
    {
    KN_OBJECT(Resource, Object)

    public:
        Resource() = default;
        virtual ~Resource() = default;

        virtual bool Valid() = 0;
        virtual void LoadResource(const String &path) = 0;

    public:
        i32 GetResourceHash() { return m_stringHash; }
        String &GetResourceLoadPath() { return m_loadPath; }

    public:
        bool operator ==(const Resource &resource) { return (resource.m_stringHash == m_stringHash); }
        bool operator >(const Resource &resource) { return (resource.m_stringHash > m_stringHash); }
        bool operator <(const Resource &resource) { return (resource.m_stringHash < m_stringHash); }

    protected:
        String m_loadPath {};
        i32 m_stringHash {0};
    };

    // its an array of resources
    // you load them all with one function call
    class LoadResourceArray
    {
    public:
        LoadResourceArray();
        ~LoadResourceArray();

        void AddResource(Resource *r, String s);
        void LoadResourceToContext(Context *context);

    private:
        ArrayList<Pair<Resource*, String>> m_resourceLoadList;
    };
}

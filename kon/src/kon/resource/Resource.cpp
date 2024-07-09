
#include "Resource.hpp"
#include <kon/core/Context.hpp>

namespace kon
{
    LoadResourceArray::LoadResourceArray() = default;
    LoadResourceArray::~LoadResourceArray() = default;

    void LoadResourceArray::AddResource(Resource *r, String s)
    {
        m_resourceLoadList.Add(
                Pair<kon::Resource*, kon::String>(r, s));
    }

    void LoadResourceArray::LoadResourceToContext(Context *context)
    {
        const u32 resources = m_resourceLoadList.Index();
        for(int i = 0; i < resources; i++)
        {
            Pair<kon::Resource*, kon::String> &rl =  m_resourceLoadList.Get(i);
            KN_TRACE("Loading Resource %i/%i [%s]", i+1, resources, rl.second.c_str());
            rl.first->LoadResource(rl.second);
            context->RegisterResource(rl.first, rl.second);
        }
    }
}

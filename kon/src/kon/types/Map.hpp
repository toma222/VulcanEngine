
#pragma once

#include <kon/core/Core.hpp>
#include <kon/types/ArrayList.hpp>
#include <kon/types/Pair.hpp>

namespace kon
{
    template<typename T>
    u32 Hash(T &hash);

    // thought that it would be fun to implement a simple unordered map and ordered map
    template<typename T>
    struct UnorderedMap
    {
    public:
        UnorderedMap() = default;
        UnorderedMap(u32 amount) { ReserveElements(amount); }

        void ReserveElements(u32 amount) { m_hashes.Reserve(amount); }

        bool Enter(T t)
        {
            m_hashes.Add(Pair<u32, T>(Hash<T>(t), t));
            return true; // idk why this is here :3
        }

        T Get(u32 hash) const
        {
            for(int i = 0; i < m_hashes.Index(); i++)
                if(m_hashes.Get(i).first == hash)
                    return m_hashes.Get(i).second;
            KN_WARN("Unordered map searched for value that does not exist");
            return m_hashes.Get(0).second;
        }

        T Get(T t) const
        {
            return Get(Hash<T>(t));
        }

        bool HasDuplicate(u32 hash) const
        {
            for(int i = 0; i < m_hashes.Index(); i++)
                if(m_hashes.Get(i).first == hash)
                    return true;
            return false;
        }

        bool HasDuplicate(T t) const
        {
            return HasDuplicate(Hash<T>(t));
        }

    public:
        ArrayList<Pair<u32, T>> m_hashes;
    };

    // takes a type T and you enter it with a defined hash
    template<typename T, typename Hash>
    class HashMap
    {
    public:
        using HashMapEntry = Pair<Hash, T>;

    public:
        HashMap() = default;
        HashMap(u32 amount) { ReserveElements(amount); }
        ~HashMap() = default;

        void ReserveElements(u32 amount) { m_hashes.Reserve(amount); }

        bool Enter(T t, Hash hash)
        {
            m_hashes.Add(
                HashMapEntry(hash, t));
            return true; // idk why this is here :3
        }

        T Get(Hash h) const
        {
            for(int i = 0; i < m_hashes.Index(); i++)
                if(m_hashes.Get(i).first == h)
                    return m_hashes.Get(i).second;

            KN_WARN("Unordered map searched for value that does not exist");
            return m_hashes.Get(0).second;
        }

        bool HasDuplicate(Hash h) const
        {
            for(int i = 0; i < m_hashes.Index(); i++)
                if(m_hashes.Get(i).first == h)
                    return true;
            return false;
        }

    public:
        ArrayList<HashMapEntry> m_hashes;
    };
}

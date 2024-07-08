
#pragma once

#include <cassert>
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
	

	template<typename T, typename Hash>
	class BetterHashMap
	{
	public:
		struct KeyValuePair
		{
			KeyValuePair(T v, const Hash h)
				: value(v), key(h) {}
			KeyValuePair(const KeyValuePair &) = default;
			~KeyValuePair() = default;

			KeyValuePair &operator=(const KeyValuePair&) = delete;

			T value;
			const Hash key;
		};

		struct Node
		{
			Node(T value, const Hash key)
				: pair(value, key) {}
			~Node()
			{
				// no seg faults
				if(a != nullptr) delete a;
				if(b != nullptr) delete b;
			}

			KeyValuePair pair;

			Node *prev {nullptr}; // prev is only nullptr if node is head
			Node *a {nullptr};
			Node *b {nullptr};
		};

		BetterHashMap() = default;
		~BetterHashMap()
		{
			delete m_head;
		}

		// returns a valid node pointer
		Node *AllocateNode(T value, Hash hash)
		{
			// we need to write an allocator class at SOME point
			return (new Node(value, hash));
		}

		// returns wether or not the operation was successful
		bool Emplace(T value, Hash hash)
		{
			if(m_head == nullptr)
			{
				m_head = AllocateNode(value, hash);
			}

			Node *current = m_head;
			bool found = false;
			while(found == false)
			{
				if(current->pair.key == hash) return false;

				// Check if we found the correct bucket
				if(current->pair.key > hash)
				{
					if(current->b == nullptr)
					{
						current->b = AllocateNode(value, hash);
						found = true;
					} else {
						current = current->b;
						continue;
					}
				}else{
					if(current->a == nullptr)
					{
						current->a = AllocateNode(value, hash);
						found = true;
					} else {
						current = current->a;
						continue;
					}
				}
			}

			return true;
		}

		T Get(Hash hash)
		{
			Node *n = FindNode(hash);
			// assert(n != nullptr);
			return n->pair.value;
		}

		// remove an entry from the table
		void Remove(Hash hash)
		{
			Node *n = FindNode(hash);
			
			// make a copy of all the nodes UNDER n
			BetterHashMap<T, Hash> copy;
			CopyHashMap(copy, n);

			delete n;

			EmplaceHashMap(m_head);
		}

		// including head node
		void CopyHashMap(BetterHashMap<T, Hash> &dst, Node *head)
		{
			// we have hit the bottom
			if(head == nullptr) return;
			dst.Emplace(head->pair.value, head->pair.key);
			CopyHashMap(dst, head->a);
			CopyHashMap(dst, head->b);
		}

		void EmplaceHashMap(Node *head)
		{
			if(head == nullptr) return;
			Emplace(head->pair.value, head->pair.key);
			EmplaceHashMap(head->a);
			EmplaceHashMap(head->b);
		}

	private:
		inline Node *FindNode(Hash hash)
		{
			Node *current = m_head;
			// bool found = false;
			while(true)
			{
				if(current->pair.key == hash) return current;

				// Check if we found the correct bucket
				if(current->pair.key > hash)
				{
					if(current->b == nullptr) return nullptr;
					current = current->b;
				}else{
					if(current->a == nullptr) return nullptr;
					current = current->a;
				}
			}
		}

	private:
		Node *m_head {nullptr};
		// Node *m_tail;
	};
}

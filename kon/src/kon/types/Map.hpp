
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
	/*
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
	*/

	template<typename T, typename Hash>
	class UnorderedMap
	{
	public:
		UnorderedMap() = default;
		~UnorderedMap() = default;
		UnorderedMap(u32 size)
			: m_array(size) {}

		T Get(Hash hash)
		{
			for(int i = 0; i < m_array.Index(); i++)
				if(m_array.Get(i).second == hash)
					return m_array[i].first;

			return m_array[0].first;
		}

		void Enter(T value, Hash hash)
		{
			m_array.Add(Pair<T, Hash>(value, hash));
		}

		bool HasDuplicate(Hash hash)
		{
			for(int i = 0; i < m_array.Index(); i++)
				if(m_array.Get(i).second == hash)
					return true;

			return false;
		}

		// T &operator[](int i) { return m_array.Get(i); }

	private:
		ArrayList<Pair<T, Hash>> m_array;
	};

	template<typename T, typename Hash>
	class HashMap
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
			Node(Node *p, T value, const Hash key)
				: pair(value, key), prev(p) {}
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

		HashMap() = default;
		~HashMap()
		{
			delete m_head;
		}

		// returns a valid node pointer
		Node *AllocateNode(Node *prev, T value, Hash hash)
		{
			// we need to write an allocator class at SOME point
			return (new Node(prev, value, hash));
		}

		// returns wether or not the operation was successful
		bool Emplace(T value, Hash hash)
		{
			if(m_head == nullptr)
			{
				m_head = AllocateNode(nullptr, value, hash);
				return true;
			}

			Node *current = m_head;
			bool found = false;
			while(found == false)
			{
				if(current->pair.key == hash)
				{
					return false;
				}

				// Check if we found the correct bucket
				if(current->pair.key > hash)
				{
					if(current->b == nullptr)
					{
						current->b = AllocateNode(current, value, hash);
						found = true;
					} else {
						current = current->b;
						continue;
					}
				}else{
					if(current->a == nullptr)
					{
						current->a = AllocateNode(current, value, hash);
						found = true;
					} else {
						current = current->a;
						continue;
					}
				}
			}

			return true;
		}

		T Get(Hash hash) const
		{
			Node *n = FindNode(hash);
			// if(n == nullptr) KN_WARN("could not find hash");
			// assert(n != nullptr);
			return n->pair.value;
		}

		bool CheckCollision(Hash hash) const
		{
			return (FindNode(hash) != nullptr);
		}

		// remove an entry from the table
		void Remove(Hash hash)
		{
			Node *n = FindNode(hash);
			if(n == nullptr) return;
			
			// make a copy of all the nodes UNDER n
			HashMap<T, Hash> copy;
			CopyHashMap(copy, n->a);
			CopyHashMap(copy, n->b);

			if(n->prev->a == n) n->prev->a = nullptr;
			if(n->prev->b == n) n->prev->b = nullptr;
			delete n;

			EmplaceHashMap(copy.m_head);
		}

		bool HasHash(Hash hash)
		{
			return (FindNode(hash) == nullptr);
		}

		// including head node
		// i dont personaly think that recursion is that big of a deal ibh
		void CopyHashMap(HashMap<T, Hash> &dst, Node *head)
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
		inline Node *FindNode(Hash hash) const
		{
			Node *current = m_head;
			// bool found = false;
			while(current != nullptr)
			{
				if(current->pair.key == hash)
				{
					return current;
				}

				// Check if we found the correct bucket
				if(current->pair.key > hash)
				{
					// if(current->b == nullptr) return nullptr;
					current = current->b;
				}else{
					// if(current->a == nullptr) return nullptr;
					current = current->a;
				}
			}

			return nullptr;
		}

	private:
		Node *m_head {nullptr};
		// Node *m_tail;
	};
}

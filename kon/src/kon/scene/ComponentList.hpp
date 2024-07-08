
#pragma once

#include "kon/scene/Scene.hpp"
#include "kon/types/ArrayList.hpp"

#include <kon/types/Array.hpp>
#include <kon/types/Map.hpp>

namespace kon
{
	template<typename T>
	class ComponentList
	{
	public:
		void EmplaceComponent(Entity entity, T component)
		{
			m_entityToIndex.Enter(m_index, entity);
			m_array[m_index] = component;
			m_index++;
		}

		void DestroyComponent(Entity entity)
		{
			// destroy the ref in the hash map
			
		}

	private:
		
		// entity to index
		HashMap<Entity, u32> m_entityToIndex;

		// index to component T
		Array<T, knMaxEntities> m_array;
		u32 m_index {0};
	};
}


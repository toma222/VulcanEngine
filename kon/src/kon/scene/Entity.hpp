
#pragma once

#include "kon/core/Core.hpp"
#include "kon/scene/Scene.hpp"
#include <utility>

namespace kon
{
	class EntityHandle
	{
	public:
		EntityHandle(Scene *scene, entt::entity entity)
			: m_scene(scene), m_entity(entity) {}
		EntityHandle() = default;

		template<typename T>
		T &GetComponent()
		{
			return m_scene->m_registry.get<T>(m_entity);
		}

		template<typename T, typename ...Args>
		void AddComponent(Args &&...args)
		{
			return m_scene->m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_scene->m_registry.any_of<T>(m_entity);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_scene->m_registry.erase<T>(m_entity);
		}

	private:
		Scene *m_scene;
		entt::entity m_entity {entt::null};
	};
}


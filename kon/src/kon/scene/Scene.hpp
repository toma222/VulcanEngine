
#pragma once

// #include "kon/scene/Entity.hpp"
#include "kon/types/Map.hpp"
#include "kon/types/String.hpp"
#include "kon/types/UUID.hpp"
#include <kon/core/Core.hpp>

// bye bye compile time
#include <entt/entt.hpp>

namespace kon
{
	class EntityHandle;

	class Scene
	{
	public:
		Scene();
		~Scene();

		EntityHandle CreateEntity(const String &name, UUID id);
		EntityHandle CreateEntity(const String &name="entity");

	public:
		entt::registry m_registry;

		friend EntityHandle;
	};
}

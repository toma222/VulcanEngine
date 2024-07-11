
#pragma once

#include "kon/scene/Entity.hpp"
#include "kon/types/Map.hpp"
#include "kon/types/String.hpp"
#include "kon/types/UUID.hpp"
#include <kon/core/Core.hpp>

// bye bye compile time
#include <entt/entt.hpp>

namespace kon
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const String &name, UUID id);
		Entity CreateEntity(const String &name="entity");

	private:
		entt::registry m_registry;
	};
}

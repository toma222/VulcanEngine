
#include "Scene.hpp"

#include <entt/entt.hpp>

namespace kon
{
	Scene::Scene()
	{
		// m_registry = new entt::registry();
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const String &name, UUID id)
	{
		// m_entityToComponentBits.Emplace(0, id.uuid);
		return id.uuid;
	}

	Entity Scene::CreateEntity(const String &name)
	{
		return CreateEntity(name, UUID());
	}
}


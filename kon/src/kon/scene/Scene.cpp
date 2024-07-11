
#include "Scene.hpp"
#include "kon/scene/Components.hpp"

#include <entt/entt.hpp>

#include <kon/scene/Entity.hpp>

namespace kon
{
	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{

	}

	EntityHandle Scene::CreateEntity(const String &name, UUID id)
	{
		EntityHandle handle{this, m_registry.create()};
		handle.AddComponent<ComponentTransform>();
		handle.AddComponent<ComponentID>(id, name);

		return handle;
	}

	EntityHandle Scene::CreateEntity(const String &name)
	{
		return CreateEntity(name, UUID());
	}
}


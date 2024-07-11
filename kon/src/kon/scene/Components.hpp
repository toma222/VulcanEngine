
#pragma once

#include "kon/types/UUID.hpp"
#include <glm/glm.hpp>
#include <kon/types/String.hpp>

namespace kon
{
	class ComponentTransform 
	{
	public:
		ComponentTransform() = default;
		ComponentTransform(glm::vec3 p, glm::vec3 r, glm::vec3 s)
			: position(p), rotation(r), scale(s) {}

		~ComponentTransform() = default;

	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	// defines everything needed for defining this component in the scene
	class ComponentID
	{
	public:
		ComponentID() = default;
		ComponentID(UUID id, String n)
			: uuid(id), name(n) {}
		~ComponentID() = default;

	public:
		UUID uuid;
		String name;
	};
}


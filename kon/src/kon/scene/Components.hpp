
#pragma once

#include <glm/glm.hpp>
#include <kon/types/String.hpp>

namespace kon
{
	class Component
	{
	public:
	};

	class ComponentTransform : public Component
	{
	public:
		ComponentTransform(glm::vec3 p, glm::vec3 r, glm::vec3 s)
			: position(p), rotation(r), scale(s) {}

		~ComponentTransform() = default;

	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	// defines everything needed for defining this component in the scene
	class ComponentID : public Component
	{
	public:
		ComponentID(String n)
			: name(n) {}
		~ComponentID() = default;

	public:
		String name;
	};

	// will define later ig
	/*
	class ComponentMeshRenderer : public Component
	{
	public:
	};
	*/
}


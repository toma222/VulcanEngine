
#pragma once

#include <kon/types/ArrayList.hpp>
#include <kon/resource/ResourceModel.hpp>

namespace kon
{
	class Mesh
	{
	public:
		Mesh();
		~Mesh();

	private:
		ArrayList<ResourceModel::ModelVertex> m_verticies;
		ArrayList<u32> m_indicies;
	};
}


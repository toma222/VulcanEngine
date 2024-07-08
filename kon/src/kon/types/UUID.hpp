
#pragma once

#include "kon/core/Core.hpp"
namespace kon
{
	// Generates a random u64
	struct UUID
	{
	public:
		UUID(u64 u)
			: uuid(u) {}
		UUID();
		~UUID() = default;
		UUID(const UUID &u) = default;

	public:
		u64 uuid;
	};
}


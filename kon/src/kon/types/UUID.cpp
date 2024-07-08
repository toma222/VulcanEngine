
#include "UUID.hpp"

#include <random>

namespace kon
{
	UUID::UUID()
	{
		static std::random_device rd;
    	static std::mt19937_64 e2(rd());
    	static std::uniform_int_distribution<long long int> dist
			(std::llround(std::pow(2,61)), std::llround(std::pow(2,62)));

		uuid = dist(e2);
	}
}


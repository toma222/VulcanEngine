
#pragma once

namespace kon
{
	template<typename T, int Length>
	class Array
	{
	public:
		Array() = default;
		~Array() = default;

		// gets T
		T Get(int i) { return m_array[i]; }

		T &operator[](int i) { return m_array[i]; }

	private:
		T m_array[Length];
	};
}


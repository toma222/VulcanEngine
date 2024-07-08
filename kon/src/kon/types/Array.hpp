
#pragma once

namespace kon
{
	template<typename T, int Length>
	class Array
	{
	public:
		Array();
		~Array();

		// gets T
		T Get(int i) { return m_array[i]; }

	private:
		T m_array[Length];
	};
}


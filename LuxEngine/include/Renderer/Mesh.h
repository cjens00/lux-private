#pragma once
#include <vector>

namespace lux
{
	class Mesh
	{
		std::vector<unsigned int> indices;
	public:
		Mesh();
		void Initialize(); // resize vector, etc.
	};
}
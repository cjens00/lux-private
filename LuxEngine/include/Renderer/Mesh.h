#pragma once
#include <vector>
#include <glm/vec3.hpp>

namespace lux
{
	class Mesh
	{
		std::vector<glm::vec3> vertex_positions;
		std::vector<glm::vec3> vertex_normals;
		std::vector<unsigned int> vertex_indices;
	public:
		Mesh();
		void Initialize(); // resize vector, etc.
	};
}

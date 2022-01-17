#pragma once

#include <vector>
#include <Renderer/Mesh.h>
#include <Renderer/Material.h>

namespace lux
{
	class Model
	{
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
	public:
		Model();
		//Model(/*from .obj overload*/);
	};
}


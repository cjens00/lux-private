#pragma once

#include <string>
#include <vector>
#include <Renderer/Mesh.h>
#include <Renderer/Material.h>

namespace lux
{
	class Model
	{
		std::string file_name;
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
	public:
		Model(std::string filename);
		void Draw();
		bool LoadObj();
	};
}


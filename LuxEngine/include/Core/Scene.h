#pragma once

#include <vector>

#include <Renderer/Model.h>

namespace lux
{
	class Scene
	{
		std::vector<lux::Model> models;
	public:
		Scene();
		void Initialize();
		bool WriteToFile();
		bool LoadFromFile();
	};
}

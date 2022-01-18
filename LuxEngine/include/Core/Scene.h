#pragma once

#ifndef __glad_h_
#include <glad/glad.h>
#endif

#include <vector>
#include <Renderer/Model.h>

namespace lux
{
	// Always owned by Renderer. TODO: Implement a Scene Manager class
	class Scene
	{
		bool isDefault = true;
		bool isActiveScene = false;
		std::string scene_name = "Default Scene";
		std::vector<Model> models = {};
	public:
		// Environment color, aka glClearColor(RGBA).
		// Default: Medium-Dark Grey
		glm::fvec4 env_color = {0.4f, 0.4f, 0.4f, 1.0f};

		// Constructor
		Scene() = default;

		// Getters
		inline std::string GetSceneName() { return scene_name; }
		inline bool GetIsActiveScene() { return isActiveScene; }
		inline bool GetIsDefault() { return isDefault; }

		// Setters
		inline void SetSceneName(std::string s_n) { scene_name = s_n; }
		inline void SetIsActiveScene(bool i_a_s) { isActiveScene = i_a_s; }

		// Called by Renderer class
		void SubmitDrawData();

		// Methods for Save/Load
		bool LoadFromFile();
		bool WriteToFile();
	};
}

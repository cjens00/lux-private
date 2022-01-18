#pragma once
#include <memory>
#include <string>
#include <vector>

#ifndef __glad_h_
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <Core/Clock.h>
#include <Renderer/Framebuffer.h>
#include <Renderer/Model.h>

#include "Core/Scene.h"

namespace lux
{
	class Renderer
	{
		// Rendering configuration
		// & OpenGL-related settings
		const int gl_version_maj = 3;
		const int gl_version_min = 3;
		std::string glsl_version = "#version 330";
		std::shared_ptr<GLFWwindow*> gl_window;
		std::vector<lux::Framebuffer> frame_buffers;

		// Scene related
		std::vector<std::string> scene_names;
		std::vector<Scene> scenes;
		Scene current_scene;

	public:
		Renderer();

		// Helpers called by the owner, Window,
		// during the initialization of Renderer
		glm::ivec2 GetGLVersion() const;
		void SetGLWindow(std::shared_ptr<GLFWwindow*> sptr_gl_window);

		// Drawing-related Member Functions
		void DrawCurrentScene();

		// Callbacks
		void AddScene(std::string file_name, Scene scene);
		void LoadScene(int id_scene, std::string scene_name);
		void Update();
	};
}

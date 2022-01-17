#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <Core/Clock.h>
#include <Renderer/Renderer.h>

namespace lux
{
	class Window
	{
		Clock clock;
		unsigned int state;
		glm::ivec2 size;
		Renderer renderer;
		std::shared_ptr<GLFWwindow*> gl_window;
	public:
		Window(int width, int height);
		void Start();
	private:
		void Initialize();
		void SetupCallbacks();
	};
}

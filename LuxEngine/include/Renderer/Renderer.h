#pragma once
#include <memory>
#include <string>
#include <vector>

#include <Core/Clock.h>
#include <Renderer/Framebuffer.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

namespace lux
{
	class Renderer
	{
		int gl_version_maj = 3;
		int gl_version_min = 3;
		std::string glsl_version = "#version 330";
		std::shared_ptr<GLFWwindow*> gl_window;
		std::vector<lux::Framebuffer> frame_buffers;
	public:
		Renderer();
		glm::ivec2 Initialize();
	};
}

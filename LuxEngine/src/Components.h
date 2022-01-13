#pragma once


// GLM
#include <glm/glm.hpp>

// GLAD & GLFW
#ifndef __glad_h_
#include <glad/glad.h>
#endif
#ifndef _glfw3_h_
#include "GLFW/glfw3.h"
#endif

// Flecs
#ifndef FLECS_H
#ifndef flecs_STATIC
#define flecs_STATIC
#endif
#include <flecs.h>
#endif

// ImGUI
#include <imgui.h>


namespace lux::components
{
	struct FPSCounter
	{
		float time_elapsed;
		int frames;
	};

	struct ImGUI
	{
		ImGuiIO imgui_io;
	};

	struct Canvas
	{
		GLFWwindow* window;
		ImGUI imgui;
		FPSCounter fps_counter;
		glm::ivec2 gl_viewport_size;
		glm::ivec2 window_size;
		int fps;
	};
}

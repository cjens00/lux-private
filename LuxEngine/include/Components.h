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

// ImGui Backends
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// Dirent
// #include <dirent.h>

// glTF 2.0 Support
#include <cgltf/cgltf.h>

// Standard Library
#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <chrono>

namespace lux::tags
{
	struct isPNG
	{
	};

	struct isLoaded
	{
	};
}

namespace lux::components
{
	struct Image
	{
		const char* file_name;
		unsigned int glTexID;
		int width;
		int height;
	};

	struct ShaderFile
	{
		const char* text;
	};

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
		GLuint sv_fbo;
		GLuint sv_rbo;
		GLuint sv_texture;

		GLFWwindow* window;

		glm::ivec2 gl_viewport_size;
		glm::ivec2 window_size;
		ImVec2 sv_size;

		ImGUI imgui;

		FPSCounter fps_counter;
		int fps;
	};

	struct GLTF
	{
		cgltf_data* data;
	};
}

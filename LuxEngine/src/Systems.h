#pragma once

// GLAD & GLFW
#ifndef __glad_h_
#include <glad/glad.h>
#endif
#ifndef _glfw3_h_
#include "GLFW/glfw3.h"
#endif

// ImGui Backends
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// Flecs
#ifndef FLECS_H
#ifndef flecs_STATIC
#define flecs_STATIC
#endif
#include <flecs.h>
#endif

// STB - Image
#include <stb_image.h>

// Standard Library
#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <chrono>

// Lux
#include "Components.h"

namespace lux
{
	void Initialize(const flecs::world& world, GLFWwindow* window);
	void LoadSystems(const flecs::world& world);
	void CleanUp();
}

namespace lux::systems::helpers
{
	void PrintDebugMessage(const char* message);
	void LoadPNGImageToRAM(const flecs::world& world, const char* filename);
	void LoadPNGImageToGPU(const flecs::world& world);
}

namespace lux::systems::callbacks
{
	void glfw_resize_viewport(GLFWwindow* window, int width, int height);
	void glfw_error(int error, const char* description);
}

namespace lux::systems::gui
{
	void DrawGUI(const flecs::world& world);
	void ShowCanvasPanel(const flecs::world& world);
	void ShowMainMenu(const flecs::world& world);
}

namespace lux::systems
{
	void UpdateCanvas(const flecs::world& world);
	void DrawScene(const flecs::world& world);
	void Render(const flecs::world& world);
}

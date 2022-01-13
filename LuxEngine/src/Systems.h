#pragma once

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
	void LoadPNGImage(const flecs::world& world, const char* filename);
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

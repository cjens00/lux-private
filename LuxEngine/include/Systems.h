#pragma once

// Lux
#include <Components.h>
#include <GUISystems.h>

namespace lux
{
	void Initialize(const flecs::world& world, GLFWwindow* window);
	void LoadSystems(const flecs::world& world);
	void CleanUp();
	void UpdateMetrics(const flecs::world& world);
	void DrawScene(const flecs::world& world);
	void Render(const flecs::world& world);
}

namespace lux::systems::helpers
{
	GLFWwindow* LoadGLFW(GLFWwindow* &w_ptr);
	bool LoadGLAD();
	void InitSVFrameBuffer(const flecs::world& world);
	void PrintDebugMessage(const char* message);
}

namespace lux::systems::callbacks
{
	void glfw_resize_viewport(GLFWwindow* window, int width, int height);
	void glfw_error(int error, const char* description);
}

namespace lux::systems
{
	void System_Draw(const flecs::world& world);
	void System_Render(const flecs::world& world);
	void System_UpdateMetrics(const flecs::world& world);
	void System_ShouldClose(const flecs::world& world);
}

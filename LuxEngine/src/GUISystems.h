#pragma once

// Lux
#include "Components.h"

namespace lux::gui
{
	void LoadImGUI(const flecs::world& world, GLFWwindow* window);
	void DrawGUI(const flecs::world& world);
	void ShowCanvasPanel(const flecs::world& world);
	void ShowSceneView(const flecs::world& world);
}
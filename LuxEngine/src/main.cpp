#include <iostream>
#include "Systems.h"

int main()
{
	flecs::world world = flecs::world();
	GLFWwindow* window = nullptr;

	lux::Initialize(world, window);
	lux::LoadSystems(world);

	while(world.progress()){}

	lux::CleanUp();

	return 0;
}
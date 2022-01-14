#pragma once

// Lux
#include "Components.h"

// STB - Image
#ifdef STB_IMAGE_IMPLEMENTATION
#undef STB_IMAGE_IMPLEMENTATION
#else
#include <stb_image.h>
#endif
#define STB_IMAGE_IMPLEMENTATION

namespace lux::file
{
	void LoadPNGImage(const flecs::world& world, const char* filename);
}


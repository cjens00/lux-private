#pragma once
#include <glm/glm.hpp>
namespace lux
{
	enum TextureMapType
	{
		Albedo = 0,
		Normal = 1,
		Roughness = 2,
		Metallic = 3,
		AmbientOcclusion = 4,
		Displacement = 5,
		Roughness_Metallic = 23
	};
	class Texture
	{
		int type;
	};
}

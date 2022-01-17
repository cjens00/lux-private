#pragma once
#include <vector>

#include <Renderer/Texture.h>
#include <Renderer/Shader.h>

namespace lux
{
	class Material
	{
		Texture Albedo;
		Texture Normal;
		Texture Roughness;
		Texture Metallic;
		Texture AmbientOcclusion;
		Texture Displacement;

		Shader Shader_;
	public:
		Material();
	};
}

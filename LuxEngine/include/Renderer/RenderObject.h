#pragma once

#include <Renderer/Model.h>

namespace lux
{
	class RenderObject
	{
		RenderObject();
	public:
		RenderObject CreateRenderObject(Model &model);
	};
}

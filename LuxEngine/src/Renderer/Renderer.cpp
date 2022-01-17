#include <Renderer/Renderer.h>

lux::Renderer::Renderer()
{
	std::vector<char> glsl_version_string = 
	{
		'#','v','e','r','s','i','o','n',' ',
		(char)(gl_version_maj),(char)(gl_version_min),'0'
	};
	glsl_version = glsl_version_string.data();

	gl_window = nullptr;
	frame_buffers = {};
}

glm::ivec2 lux::Renderer::GetGLVersion() const
{
	return glm::ivec2{gl_version_maj,gl_version_min};
}

void lux::Renderer::SetGLWindow(std::shared_ptr<GLFWwindow*> sptr_gl_window)
{
	std::weak_ptr weak_gl_window = sptr_gl_window;
	this->gl_window = weak_gl_window.lock();
}

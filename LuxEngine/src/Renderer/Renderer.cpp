#include <iostream>
#include <Renderer/Renderer.h>

lux::Renderer::Renderer()
{
	std::vector<char> glsl_version_string =
	{
		'#', 'v', 'e', 'r', 's', 'i', 'o', 'n', ' ',
		(char)(gl_version_maj), (char)(gl_version_min), '0'
	};
	glsl_version = glsl_version_string.data();
	gl_window = nullptr;
	frame_buffers = {};
}

glm::ivec2 lux::Renderer::GetGLVersion() const
{
	return glm::ivec2{gl_version_maj, gl_version_min};
}

void lux::Renderer::SetGLWindow(std::shared_ptr<GLFWwindow*> sptr_gl_window)
{
	std::weak_ptr weak_gl_window = sptr_gl_window;
	this->gl_window = weak_gl_window.lock();
}

void lux::Renderer::Update()
{
	DrawCurrentScene();

	glfwSwapBuffers(*gl_window);
	glfwPollEvents();
}

void lux::Renderer::DrawCurrentScene()
{
	if (current_scene.GetIsActiveScene())
	{
		// Need some methods inside Scene to draw stuff
		// "SubmitDrawData()" or something
		current_scene.SubmitDrawData();
	}
	else
	{
		glClearColor(0.4f, 0.5f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Draw some text like "No Scene Loaded" or generic viewport, etc.
	}
}

void lux::Renderer::AddScene(std::string file_name = {}, Scene scene = {})
{
	if (file_name.empty() && (scene.GetIsDefault()))
	{
		std::cout <<
			"AddScene() requires *either* a valid file"
			"name pointing to a valid\nscene file, OR a"
			" name and a valid &Scene argument."
			<< std::endl;
	}
	else if (file_name.empty())
	{
		// Load Scene from runtime data
		scene_names.emplace_back(std::move(scene.GetSceneName()));
		scenes.emplace_back(std::move(scene));
	}
	else if (scene.GetIsDefault() == false)
	{
		// Load Scene from file
	}
}

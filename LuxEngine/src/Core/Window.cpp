#include <iostream>
#include <Core/Window.h>

lux::Window::Window(int width, int height)
{
	size.x = width;
	size.y = height;

	clock = {};
	state = 0;

	gl_window = nullptr;
	this->Initialize();
}


void lux::Window::Initialize()
{
	glm::ivec2 gl_version = renderer.GetGLVersion();

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_version.x);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_version.y);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	gl_window = std::make_shared<GLFWwindow*>
	(
		glfwCreateWindow(size.x, size.y,
		                 "XLV [Untitled]", nullptr, nullptr)
	);

	if (gl_window == nullptr)
	{
		std::cerr << "Failed to initialize GLFW window." << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent((GLFWwindow*)(gl_window.get()));

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD Loader." << std::endl;
	}

	glViewport(0, 0, size.x, size.y);
	SetupCallbacks();
	renderer.SetGLWindow(gl_window);
}

void lux::Window::SetupCallbacks()
{
	glfwSetFramebufferSizeCallback((GLFWwindow*)(gl_window.get()), [](GLFWwindow* window, int width, int height)
	{
			glViewport(0, 0, width, height);
	});

	glfwSetErrorCallback([](int ec, const char* description)
	{
			std::cerr << ec << description << std::endl;
	});
}

void lux::Window::Update()
{



	Update();
}

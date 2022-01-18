#include <iostream>
#include <Core/Window.h>

lux::Window::Window(int width, int height)
{
	size.x = width;
	size.y = height;
	state = 0;
	gl_window = nullptr;
	clock = {60.0f};
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

	if (*gl_window == nullptr)
	{
		std::cerr << "Failed to initialize GLFW window." << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(*gl_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD Loader." << std::endl;
	}

	glViewport(0, 0, size.x, size.y);
	renderer.SetGLWindow(gl_window);
	SetupCallbacks();
}

void lux::Window::SetupCallbacks()
{
	glfwSetFramebufferSizeCallback(*gl_window, [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	});

	glfwSetErrorCallback([](int ec, const char* description)
	{
		std::cerr << ec << description << std::endl;
	});
	clock.AddCallback(Clock_States::Update, [&]()
	{
		renderer.Update();
	});
	// clock.AddCallback(Clock_States::Start, [&]()
	// {
	// });
}

void lux::Window::Start()
{
	while (!glfwWindowShouldClose(*gl_window))
	{
		clock.Run();
	}
}

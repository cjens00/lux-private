#include <Systems.h>
#include <iostream>
#include <string>
#include <format>
#include <chrono>

void lux::systems::callbacks::glfw_resize_viewport(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void lux::systems::callbacks::glfw_error(int error, const char* description)
{
	fprintf_s(stderr, "GLFW::Error [%d]: %s", error, description);
}

void lux::Initialize(const flecs::world& world, GLFWwindow* window)
{
	// --- Initialize GLFW and GLFW Window ---
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(1920, 1200,
	                          "Lux", nullptr, nullptr);
	if (window == nullptr)
	{
		fprintf_s(stderr, "Lux::Error: %s", "Failed to initialize GLFW window.");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	// --- Load GLAD Wrapper ---
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf_s(stderr, "Lux::Error: %s", "Failed to load GLAD.");
		return;
	}
	glViewport(0, 0, 1920, 1200);
	glfwSetFramebufferSizeCallback(window, lux::systems::callbacks::glfw_resize_viewport);
	glfwSetErrorCallback(lux::systems::callbacks::glfw_error);

	// --- Initialize ImGUI ---
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// Configure Style & Flags
	io.FontGlobalScale = 2.0f;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();

	// --- Initialize the Canvas Singleton in ECS ---
	lux::components::Canvas canvas =
	{
		window, {io},
		{0.0, 0},
		{1920, 1200},
		{1920, 1200}, {0}
	};
	world.set<components::Canvas>(canvas);

	// Setup ImGUI Platform/Renderer Backends
	ImGui_ImplGlfw_InitForOpenGL(canvas.window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void lux::LoadSystems(const flecs::world& world)
{
	world.set_target_fps(60.0);

	auto draw = world.system<components::Canvas>().kind(flecs::PreUpdate).each(
		[](flecs::entity e, components::Canvas& c)
		{
			lux::systems::DrawScene(e.world());
			lux::systems::gui::DrawGUI(e.world());
			systems::helpers::PrintDebugMessage("systems::Draw");
		});

	auto render = world.system<components::Canvas>().kind(flecs::OnUpdate).each(
		[](flecs::entity e, components::Canvas& c)
		{
			lux::systems::Render(e.world());
			systems::helpers::PrintDebugMessage("systems::Render");
		});

	auto update_canvas = world.system<components::Canvas>().kind(flecs::PostUpdate).each(
		[](flecs::entity e, components::Canvas& c)
		{
			systems::UpdateCanvas(e.world());
			systems::helpers::PrintDebugMessage("systems::UpdateCanvas");
		});

	auto should_close = world.system<components::Canvas>().kind(flecs::OnStore).each(
		[](flecs::entity e, components::Canvas& c)
		{
			if (glfwWindowShouldClose(c.window))
			{
				systems::helpers::PrintDebugMessage("systems::should_close::true");
				e.world().deactivate_systems();
			}
			systems::helpers::PrintDebugMessage("systems::should_close::false");
		});

	draw.run();
	render.run();
	update_canvas.run();
	should_close.run();
}


void lux::CleanUp()
{
	// Clean-Up ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Clean-Up GLFW
	glfwTerminate();
}


void lux::systems::gui::DrawGUI(const flecs::world& world)
{
	// ImGUI --------------------
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create GUI
	// Draw windows here
	{
		ImGui::Begin("Demo window");
		ImGui::Button("Hello!");
		ImGui::End();
	}
	lux::systems::gui::ShowCanvasPanel(world);

	// Render GUI
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void lux::systems::gui::ShowCanvasPanel(const flecs::world& world)
{
	int x{}, y{};
	ImGui::Begin("Canvas Data View");
	ImGui::Text("Window Size: x:%d y:%d", x, y);
	ImGui::End();
}

void lux::systems::DrawScene(const flecs::world& world)
{
	// GLFW ---------------------
	glfwPollEvents();
	glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT);

	// TODO: Eventually, the rest of the scene needs to be drawn here:
	// ---------------------------------------------------------------
}

void lux::systems::Render(const flecs::world& world)
{
	auto c = world.get_mut<components::Canvas>();
	glfwSwapBuffers(c->window);
}


void lux::systems::UpdateCanvas(const flecs::world& world)
{
	auto c = world.get_mut<components::Canvas>();

	glfwGetWindowSize(c->window, &c->window_size.x, &c->window_size.y);
	glfwGetFramebufferSize(c->window, &c->gl_viewport_size.x, &c->gl_viewport_size.y);
	// Update FPS using frame and frame time counter
	if (c->fps_counter.time_elapsed > 1.0f)
	{
		c->fps = c->fps_counter.frames;
		c->fps_counter.time_elapsed = 0.0f;
		c->fps_counter.frames = 0;
	}
	c->fps_counter.time_elapsed += world.delta_time();
}

void lux::systems::helpers::PrintDebugMessage(const char* message)
{
	auto now = std::chrono::system_clock::now();
	std::cout << std::format("Debug[{0}]: {1}", now, message) << std::endl;
}

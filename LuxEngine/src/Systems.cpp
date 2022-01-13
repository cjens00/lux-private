#include <Systems.h>
#ifdef STB_IMAGE_IMPLEMENTATION
#undef STB_IMAGE_IMPLEMENTATION
#endif
#define STB_IMAGE_IMPLEMENTATION

void lux::systems::helpers::PrintDebugMessage(const char* message)
{
	using namespace std::chrono;
	auto now = floor<milliseconds>(system_clock::now());
	std::cout << std::format("[{0}]: {1}", now, message) << std::endl;
}

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
	window = glfwCreateWindow(2560, 1600,
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
	glViewport(0, 0, 2560, 1600);
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
		{2560, 1200},
		{2560, 1600}, {0}
	};
	world.set<components::Canvas>(canvas);

	// Setup ImGUI Platform/Renderer Backends
	ImGui_ImplGlfw_InitForOpenGL(canvas.window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void lux::LoadSystems(const flecs::world& world)
{
	world.set_target_fps(60.0);
	lux::systems::helpers::LoadPNGImage(world,
		"D:\\GameDevelopment\\Engine\\LuxEngine\\LuxEngine\\res\\textures\\image.png");
	auto draw = world.system<components::Canvas>().kind(flecs::PreUpdate).each(
		[](flecs::entity e, components::Canvas& c)
		{
			lux::systems::DrawScene(e.world());
			lux::systems::gui::DrawGUI(e.world());
			systems::helpers::PrintDebugMessage("systems::DrawScene");
			systems::helpers::PrintDebugMessage("systems::gui::DrawGUI");
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
				e.world().quit();
			}
			else
			{
				systems::helpers::PrintDebugMessage("systems::should_close::false");
			}
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
	c->fps_counter.frames++;
	c->fps_counter.time_elapsed += world.delta_time();
}

void lux::systems::gui::DrawGUI(const flecs::world& world)
{
	// ImGUI --------------------
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create GUI
	lux::systems::gui::ShowCanvasPanel(world);
	lux::systems::gui::ShowMainMenu(world);

	// Render GUI
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void lux::systems::gui::ShowCanvasPanel(const flecs::world& world)
{
	auto c = *world.get<components::Canvas>();
	ImGui::Begin("Canvas Data View");
	ImGui::Text("GLFW Window Address: %p", c.window);
	ImGui::Text("Window Size: x:%d y:%d", c.window_size.x, c.window_size.y);
	ImGui::Text("Viewport Size: x:%d y:%d", c.gl_viewport_size.x, c.gl_viewport_size.y);
	ImGui::Text("FPS: %i", c.fps);
	ImGui::End();
}

void lux::systems::gui::ShowMainMenu(const flecs::world& world)
{
	ImGui::Begin("Menu");
	world.each<components::Image>([](components::Image& i)
	{
		ImGui::Image(ImTextureID(i.glTexID), ImVec2(i.width, i.height));
	});
	ImGui::End();
}

void lux::systems::helpers::LoadPNGImage(const flecs::world& world, const char* filename)
{
	int num_channels = 0;
	auto image_entity = world.entity();
	auto image = lux::components::Image{filename, 0, 0};
	image_entity.add<tags::isPNG>();

	FILE* imfile;
	fopen_s(&imfile, filename, "rb");

	if (imfile == nullptr)
	{
		std::cerr << std::format("Could not open [%s].", filename) << std::endl;
		image_entity.destruct();
	}
	else
	{
		auto stb_img = stbi_load_from_file(imfile, &image.width,
		                                   &image.height, &num_channels, 4);

		// Now load into GPU
		unsigned int glTextureID = 0;
		glGenTextures(1, &glTextureID);
		glBindTexture(GL_TEXTURE_2D, glTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, stb_img);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Free up the memory used by stb_image's loader
		stbi_image_free(stb_img);

		// Lastly, add the component with the finalized data
		image_entity.set<lux::components::Image>({
			filename, glTextureID, image.width, image.height
		});
	}
}

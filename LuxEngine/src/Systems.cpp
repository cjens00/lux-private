#include <Systems.h>
using namespace lux;

void lux::systems::helpers::PrintDebugMessage(const char* message)
{
	using namespace std::chrono;
	auto now = floor<milliseconds>(system_clock::now());
	std::cout << std::format("[{0}]: {1}", now, message) << std::endl;
}

GLFWwindow* systems::helpers::LoadGLFW(GLFWwindow* &w_ptr)
{
	// --- Initialize GLFW and GLFW Window ---
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	w_ptr = glfwCreateWindow(2560, 1600,
		"Lux", nullptr, nullptr);

	if (w_ptr == nullptr)
	{
		fprintf_s(stderr, "Lux::Error: %s", "Failed to initialize GLFW window.");
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(w_ptr);

	if (!LoadGLAD())
	{
		return nullptr;
	
	}

	glViewport(0, 0, 2560, 1600);
	glfwSetFramebufferSizeCallback(w_ptr, systems::callbacks::glfw_resize_viewport);
	glfwSetErrorCallback(systems::callbacks::glfw_error);
	return w_ptr;
}

bool systems::helpers::LoadGLAD()
{
	// --- Load GLAD Wrapper ---
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf_s(stderr, "Lux::Error: %s", "Failed to load GLAD.");
		return false;
	}
	else 
		return true;
}

void systems::helpers::InitSVFrameBuffer(const flecs::world& world)
{
	auto c = world.get_mut<components::Canvas>();

	// Setup sv_framebuffer ----------------------------------------
	glGenFramebuffers(1, &c->sv_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, c->sv_fbo);

	glGenTextures(1, &c->sv_texture);
	glBindTexture(GL_TEXTURE_2D, c->sv_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, c->sv_size.x,
		c->sv_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, c->sv_texture, 0);

	glGenRenderbuffers(1, &c->sv_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, c->sv_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, c->sv_size.x, c->sv_size.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, c->sv_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// --------------------------------------------------------------
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
	// Load GLFW
	if (systems::helpers::LoadGLFW(window) == nullptr)
	{
		return;
	}
	
	// Load ImGUI
	lux::gui::LoadImGUI(world, window);

	// Create separate buffer for scene viewer
	systems::helpers::InitSVFrameBuffer(world);
}

void lux::LoadSystems(const flecs::world& world)
{
	world.set_target_fps(120.0);

	systems::System_Draw(world);
	systems::System_Render(world);
	systems::System_UpdateMetrics(world);
	systems::System_ShouldClose(world);
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

void lux::DrawScene(const flecs::world& world)
{
	auto c = world.get_mut<components::Canvas>();

	// GLFW ---------------------
	glBindFramebuffer(GL_FRAMEBUFFER, c->sv_fbo);
	glfwPollEvents();
	glClearColor(0.95f, 0.55f, 0.40f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void lux::Render(const flecs::world& world)
{
	auto c = world.get_mut<components::Canvas>();
	glfwSwapBuffers(c->window);
}

void lux::UpdateMetrics(const flecs::world& world)
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

// Flecs Systems ------------------

void systems::System_Draw(const flecs::world& world)
{
	auto draw = world.system<components::Canvas>().kind(flecs::PreUpdate).each(
		[](flecs::entity e, components::Canvas& c)
		{
			lux::DrawScene(e.world());
			lux::gui::DrawGUI(e.world());

			systems::helpers::PrintDebugMessage("systems::DrawScene");
			systems::helpers::PrintDebugMessage("systems::gui::DrawGUI");
		});
	draw.run();
}

void systems::System_Render(const flecs::world& world)
{
	auto render = world.system<components::Canvas>().kind(flecs::OnUpdate).each(
		[](flecs::entity e, components::Canvas& c)
		{
			lux::Render(e.world());

			systems::helpers::PrintDebugMessage("systems::Render");
		});
	render.run();
}

void systems::System_UpdateMetrics(const flecs::world& world)
{
	auto update_canvas = world.system<components::Canvas>().kind(flecs::PostUpdate).each(
		[](flecs::entity e, components::Canvas& c)
		{
			lux::UpdateMetrics(e.world());
			systems::helpers::PrintDebugMessage("systems::UpdateCanvas");
		});
	update_canvas.run();
}

void systems::System_ShouldClose(const flecs::world& world)
{
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
	should_close.run();
}

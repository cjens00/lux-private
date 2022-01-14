#include <GUISystems.h>
using namespace lux;

void gui::LoadImGUI(const flecs::world& world, GLFWwindow* window)
{
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
		{},{}, {}, window,
		{2560, 1600},
		{2560, 1600},{1600, 1080},
		{0,0},{}, {0.0, 0},
		{0}
	};
	world.set<components::Canvas>(canvas);

	// Setup ImGUI Platform/Renderer Backends
	ImGui_ImplGlfw_InitForOpenGL(canvas.window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void lux::gui::DrawGUI(const flecs::world& world)
{
	glClearColor(0.55f, 0.65f, 0.70f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT);

	// ImGUI --------------------
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create GUI
	lux::gui::ShowCanvasPanel(world);
	lux::gui::ShowSceneView(world);

	// Render GUI
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void lux::gui::ShowCanvasPanel(const flecs::world& world)
{
	auto c = *world.get<components::Canvas>();
	ImGui::Begin("Canvas Data View");
	ImGui::Text("GLFW Window Address: %p", c.window);
	ImGui::Text("Window Size: x:%d y:%d", c.window_size.x, c.window_size.y);
	ImGui::Text("Viewport Size: x:%d y:%d", c.gl_viewport_size.x, c.gl_viewport_size.y);
	ImGui::Text("FPS: %i", c.fps);
	ImGui::End();
}

void lux::gui::ShowSceneView(const flecs::world& world)
{
	auto c = world.get_mut<components::Canvas>();
	
	ImGui::Begin("Scene View");
	ImGui::BeginChild("sv_image", ImGui::GetContentRegionAvail());

	// TODO: DON'T FORGET TO UPDATE ACTUAL GLVIEWPORT SIZE/COORDS
	// TODO: WITH TOP LEFT POS OF CHILD FRAME AND NEW_SV_SIZE!!

	auto new_sv_size = ImGui::GetContentRegionAvail();
	c->sv_size = { new_sv_size.x, new_sv_size.y };

	ImGui::Image(ImTextureID(c->sv_texture), new_sv_size);

	ImGui::EndChild();
	ImGui::End();
}

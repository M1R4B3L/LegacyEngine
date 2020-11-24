#include "Globals.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "Primitive.h"
#include "imgui.h"
#include "examples\imgui_impl_sdl.h"
#include "examples\imgui_impl_opengl3.h"
#include "Importer.h"
#include "GameObjects.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ModuleSceneIntro.h"


ModuleEditor::ModuleEditor(bool start_enable) : Module(start_enable),
about_window(false), config_window(false), console_window(true), inspector_window(true), hierarchy_window(true), demo_window(false), docking_window(true),
org("CITM"), scroll(true)
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	io.ConfigDockingWithShift = false;

	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init("#version 460"); //TODO: glsl version ¿?¿?

	return true;
}

update_status ModuleEditor::Update(float dt)
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	WindowDocking();
	WindowAbout();
	WindowConfig();
	WindowConsole();
	WindowInspector();
	WindowHierarchy();
	WindowDemo();

	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	return false;
}

void ModuleEditor::WindowDocking()
{

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	if (ImGui::Begin("DockSpace", &docking_window, window_flags)) {
		// DockSpace
		ImGui::PopStyleVar(3);
		if (docking_window)
		{
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
			MenuBar();
		}
		ImGui::End();
	}

}

void ModuleEditor::MenuBar()
{

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Scene", "")) {
			}
			if (ImGui::MenuItem("Load Scene", "")) {
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			if (ImGui::MenuItem("Save Scene", "")) {
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			if (ImGui::MenuItem("New Project", "")) {
			}
			if (ImGui::MenuItem("Open Project", "")) {
			}
			if (ImGui::MenuItem("Save Project", "")) {
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			if (ImGui::MenuItem("Exit", "ESC")) {
				App->close_app = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Project Settings...", NULL, config_window)) {
				config_window = !config_window;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Create")) {
			if (ImGui::BeginMenu("Primitives")) {
				if (ImGui::MenuItem("Cube")) {
					
				}
				if (ImGui::MenuItem("Sphere")) {

				}
				if (ImGui::MenuItem("Piramid")) {

				}
				if (ImGui::MenuItem("Cylinder")) {

				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::BeginMenu("Layouts", NULL)) {

				ImGui::EndMenu();
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			if (ImGui::BeginMenu("General")) {
				if (ImGui::MenuItem("Scene")) {

				}
				if (ImGui::MenuItem("Insperctor", NULL, inspector_window)) {
					inspector_window = !inspector_window;
				}
				if (ImGui::MenuItem("Hierarchy", NULL, hierarchy_window)) {
					hierarchy_window = !hierarchy_window;
				}
				if (ImGui::MenuItem("Console", NULL, console_window)) {
					console_window = !console_window;
				}
				ImGui::EndMenu();
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			if (ImGui::MenuItem("Demo", NULL, demo_window)) {
				demo_window = !demo_window;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("About Legacy Engine")) {
				about_window = !about_window;
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			if (ImGui::MenuItem("Download Latest")) {
				ShellExecuteA(NULL, "open", "https://github.com/M1R4B3L/LegacyEngine/releases", NULL, NULL, 3);
			}
			if (ImGui::MenuItem("Report a bug")) {
				ShellExecuteA(NULL, "open", "https://github.com/M1R4B3L/LegacyEngine/issues", NULL, NULL, 3);
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			if (ImGui::MenuItem("Documentation")) {
				ShellExecuteA(NULL, "open", "https://github.com/M1R4B3L/LegacyEngine", NULL, NULL, 3);
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void ModuleEditor::WindowAbout()
{
	if (about_window) {
		if (ImGui::Begin("About", &about_window))
		{
			ImGui::BulletText("Legacy Engine:");
			ImGui::Separator();
			ImGui::Text("Developed by Jordi Bach & Adrian Mirabel,\nfor engine subject in CITM Barcelona.");
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::BulletText("Libraries:");
			ImGui::Separator();
			ImGui::Text("STL Wiki");
			ImGui::SameLine();
			if (ImGui::Button("STL")) {
				ShellExecuteA(NULL, "open", "https://www.geeksforgeeks.org/the-c-standard-template-library-stl/?ref=lbp", NULL, NULL, 3);
			}
			ImGui::Spacing();
			ImGui::Text("SDL v2.0.12");
			ImGui::SameLine();
			if (ImGui::Button("SDL")) {
				ShellExecuteA(NULL, "open", "https://www.libsdl.org/download-2.0.php", NULL, NULL, 3);
			}
			ImGui::Spacing();
			ImGui::Text("OpenGL v3.1.0");
			ImGui::SameLine();
			if (ImGui::Button("OpenGL")) {
				ShellExecuteA(NULL, "open", "https://www.opengl.org/", NULL, NULL, 3);
			}
			ImGui::Spacing();
			ImGui::Text("Glew v2.1.0");
			ImGui::SameLine();
			if (ImGui::Button("Glew")) {
				ShellExecuteA(NULL, "open", "http://glew.sourceforge.net/", NULL, NULL, 3);
			}
			ImGui::Spacing();
			ImGui::Text("ImGui v1.79");
			ImGui::SameLine();
			if (ImGui::Button("ImGui")) {
				ShellExecuteA(NULL, "open", "https://github.com/ocornut/imgui", NULL, NULL, 3);
			}
			ImGui::Spacing();
			ImGui::Text("Assimp v3.1.1");
			ImGui::SameLine();
			if (ImGui::Button("Assimp")) {
				ShellExecuteA(NULL, "open", "https://github.com/assimp/assimp", NULL, NULL, 3);
			}
			ImGui::Spacing();
			ImGui::Text("Devil v1.8.0");
			ImGui::SameLine();
			if (ImGui::Button("Devil")) {
				ShellExecuteA(NULL, "open", "http://openil.sourceforge.net/download.php", NULL, NULL, 3);
			}
			ImGui::Spacing();
			ImGui::Text("MathGeoLib v1.5");
			ImGui::SameLine();
			if (ImGui::Button("MathGeoLib")) {
				ShellExecuteA(NULL, "open", "https://github.com/juj/MathGeoLib", NULL, NULL, 3);
			}
			if (ImGui::CollapsingHeader("License"))
			{
				ImGui::Text("MIT License\n"
					"Copyright(c) 2020 Legacy Engine\n"
					"Permission is hereby granted, free of charge, to any person obtaining a copy\n"
					"of this softwareand associated documentation files(the ""Software""), to deal\n"
					"in the Software without restriction, including without limitation the rights\n"
					"to use, copy, modify, merge, publish, distribute, sublicense, and /or sell\n"
					"copies of the Software, and to permit persons to whom the Software is\n"
					"furnished to do so, subject to the following conditions :\n"
					"The above copyright noticeand this permission notice shall be included in all\n"
					"copies or substantial portions of the Software.\n"
					"THE SOFTWARE IS PROVIDED ""AS IS"", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
					"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
					"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\n"
					"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
					"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
					"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
					"SOFTWARE.");
			}
			//libraries, 
		}
		ImGui::End();
	}
}

void ModuleEditor::WindowConfig()
{
	if (config_window) {
		//ImGuiCond_Once;
		//ImGui::SetNextWindowPos()
		if (ImGui::Begin("Configuration", &config_window, ImGuiWindowFlags_MenuBar)) {

			ImGui::BeginMenuBar();
			if (ImGui::MenuItem("Save"))
			{

			}
			if (ImGui::MenuItem("Reset"))
			{

			}
			if (ImGui::MenuItem("SetAsDefault"))
			{

			}
			ImGui::EndMenuBar();

			ImGui::Spacing();
			if (ImGui::BeginTabBar(""))
			{
				if (ImGui::BeginTabItem("Display")) {
					if (ImGui::CollapsingHeader("Window"))
					{

						static float brightness = App->window->GetBrightness();
						ImGui::Text("Brightness: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(255, 0, 255, 255), "%.3f", brightness);
						ImGui::SliderFloat("Brightness", &brightness, 0, 1, "%.3f");
						App->window->SetBrightness(brightness);

						//static float gamma = App->window->GetGamma();

						static int width = App->window->GetWidth();
						static int height = App->window->GetHeight();
						ImGui::Separator();
						ImGui::Text("Windows Size: You should not use this");
						ImGui::Text("Width ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(255, 0, 255, 255), "%d ", width);
						ImGui::Text("Height ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(255, 0, 255, 255), "%d ", height);
						ImGui::SliderInt("Width", &width, 640, 3840);
						ImGui::SliderInt("Height", &height, 480, 2160);
						App->window->SetSize(width, height);
						ImGui::Separator();
						ImGui::Text("Refresh date");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(255, 0, 255, 255), "%d", App->window->GetRefreshRate());

						static bool fullscreen = App->window->GetFullscreen();
						static bool resizable = App->window->GetResizable();
						static bool bordeless = App->window->GetBorderless();
						static bool fulldesktop = App->window->GetFullscreenDesktop();

						if (ImGui::Checkbox("Fullscreen", &fullscreen))
						{
							App->window->SetFullscreen(fullscreen);
						}
						ImGui::SameLine();
						if (ImGui::Checkbox("Resizable", &resizable))
						{
							App->window->SetResizable(resizable);
						}

						if (ImGui::Checkbox("Borderless", &bordeless))
						{
							App->window->SetBorderless(bordeless);
						}
						ImGui::SameLine();
						if (ImGui::Checkbox("Full Desktop", &fulldesktop))
						{
							App->window->SetFullscreenDesktop(fulldesktop);
						}
					}
					if (ImGui::CollapsingHeader("Application"))
					{
						static char name[128];
						strcpy_s(name, 128, App->GetEngineTitle());
						if (ImGui::InputText("App Name", name, 128, ImGuiInputTextFlags_EnterReturnsTrue))  //We can use ImGuiInputTextFlags_EnterReturnsTrue only if we want to change the name if enter is pressed
							App->SetEngineTitle(name);

						ImGui::InputText("Organization", org, 128);

						ImGui::Separator();
						ImGui::Spacing();

						int max_fps = App->GetFramerate();
						if (ImGui::SliderInt("Max FPS", &max_fps, 0, 144))
							App->CapFramerate(max_fps);

						ImGui::Text("Limit Framerate:");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(0, 255, 255, 255), "%u", App->GetFramerate());

						char title[25];
						sprintf_s(title, 25, "Framerate %.1f", App->fps_log[App->fps_log.size() - 1]);
						ImGui::PlotHistogram("##framerate", &App->fps_log[0], App->fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
						sprintf_s(title, 25, "Milliseconds %.1f", App->ms_log[App->ms_log.size() - 1]);
						ImGui::PlotHistogram("##milliseconds", &App->ms_log[0], App->ms_log.size(), 0, title, 0.0f, 50.0f, ImVec2(310, 100));

						//Falta memory pero hem de possar la llibreria del Ric
					}
					ImGui::EndTabItem();

				}

				if (ImGui::BeginTabItem("Render"))
				{

					ImGui::Text("Renderer Options");
					ImGui::Separator();
					ImGui::Spacing();
					bool depth_test = App->renderer3D->GetglEnableFlags(GL_DEPTH_TEST);
					bool cull_face = App->renderer3D->GetglEnableFlags(GL_CULL_FACE);
					bool lightning = App->renderer3D->GetglEnableFlags(GL_LIGHTING);
					bool color_material = App->renderer3D->GetglEnableFlags(GL_COLOR_MATERIAL);
					bool texture2D = App->renderer3D->GetglEnableFlags(GL_TEXTURE_2D);
					bool show_wireframes = App->renderer3D->GetWireframes();
					ImGui::BulletText("General");
					if (ImGui::Checkbox("Depth", &depth_test))
					{
						App->renderer3D->SetglEnbleFlags(GL_DEPTH_TEST, depth_test);
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Cull Face", &cull_face))
					{
						App->renderer3D->SetglEnbleFlags(GL_CULL_FACE, cull_face);
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Lightning", &lightning))
					{
						App->renderer3D->SetglEnbleFlags(GL_LIGHTING, lightning);
					}
					ImGui::BulletText("Polygon Mode");
					if (ImGui::Checkbox("Wireframes", &show_wireframes))
					{
						App->renderer3D->SetWireframes(show_wireframes);
					}
					ImGui::BulletText("Material");
					if (ImGui::Checkbox("Color Material", &color_material))
					{
						App->renderer3D->SetglEnbleFlags(GL_COLOR_MATERIAL, color_material);
					}
					ImGui::BulletText("Texture");
					if (ImGui::Checkbox("Texture 2D", &texture2D))
					{
						App->renderer3D->SetglEnbleFlags(GL_TEXTURE_2D, texture2D);
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Controls"))
				{
					if (ImGui::CollapsingHeader("Input"))
					{

					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Software"))
				{
					static Hardware stats;
					SDL_GetVersion(&stats.H_SDLVersion);
					stats.H_CPU = SDL_GetCPUCount();
					stats.H_CPUCache = SDL_GetCPUCacheLineSize();
					stats.H_SystemRAM = (float)SDL_GetSystemRAM() / 1024.f; //This is returned in Mb thats why we divide
					stats.H_rdtsc = SDL_HasRDTSC() == SDL_TRUE;
					stats.H_3dnow = SDL_Has3DNow() == SDL_TRUE;
					stats.H_altivec = SDL_HasAltiVec() == SDL_TRUE;
					stats.H_avx = SDL_HasAVX() == SDL_TRUE;
					stats.H_avx2 = SDL_HasAVX2() == SDL_TRUE;
					stats.H_mmx = SDL_HasMMX() == SDL_TRUE;
					stats.H_sse = SDL_HasSSE() == SDL_TRUE;
					stats.H_sse2 = SDL_HasSSE2() == SDL_TRUE;
					stats.H_sse3 = SDL_HasSSE3() == SDL_TRUE;
					stats.H_sse41 = SDL_HasSSE41() == SDL_TRUE;
					stats.H_sse42 = SDL_HasSSE42() == SDL_TRUE;

					ImGui::Text("SDL Version:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 255, 0, 255), "%d.%d.%d", stats.H_SDLVersion.major, stats.H_SDLVersion.minor, stats.H_SDLVersion.patch);
					ImGui::Spacing();
					ImGui::Text("OpenGL Version:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 255, 0, 255), "%s", glGetString(GL_VERSION));
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::Text("CPUs:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 255, 0, 255), "%u (Cache: %ukb)", stats.H_CPU, stats.H_CPUCache);
					ImGui::Text("System RAM:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 255, 0, 255), "%.2fGb", stats.H_SystemRAM);
					ImGui::Text("Features:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 255, 0, 255), "%s%s%s%s%s",
						stats.H_rdtsc ? "RDTSC " : "",
						stats.H_3dnow ? "3DNOW " : "",
						stats.H_altivec ? "ALTIVEC " : "",
						stats.H_avx ? "AVX " : "",
						stats.H_avx2 ? "AVX2 " : ""
					);
					ImGui::TextColored(ImVec4(0, 255, 0, 255), "%s%s%s%s%s%s",
						stats.H_mmx ? "MMX " : "",
						stats.H_sse ? "SSE " : "",
						stats.H_sse2 ? "SSE2 " : "",
						stats.H_sse3 ? "SSE3 " : "",
						stats.H_sse41 ? "SSE41 " : "",
						stats.H_sse42 ? "SSE42 " : ""
					);
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::Text("GPU:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 255, 0, 255), "");
					ImGui::Text("Brand:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 255, 0, 255), "");
					ImGui::Text("VRAM Budget:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 255, 0, 255), "");
					ImGui::Text("VRAM Usage:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 255, 0, 255), "");
					ImGui::Text("VRAM Available:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 255, 0, 255), "");
					ImGui::Text("VRAM Reserved:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 255, 0, 255), "");

					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::End();
		}
	}

}

void ModuleEditor::WindowConsole()
{
	if (console_window)
	{
		if (ImGui::Begin("Console", &console_window)) {


			for (int i = 0; i < string_log.size(); ++i)
			{
				ImGui::TextUnformatted(string_log[i]);
			}
			if (scroll == true)
			{
				ImGui::SetScrollHere(1.0f);

				scroll = false;
			}
			ImGui::End();
		}
	}
}

void ModuleEditor::WindowInspector()
{
	if (inspector_window)
	{
		if (ImGui::Begin("Inspector", &inspector_window))
		{
			if (ImGui::CollapsingHeader("Transform"))
			{
				
				static float position_x[3] = { 0.0f,0.0f,0.0f };

				ImGui::DragFloat3("Position", position_x, 1.0f, 0.0f, 100.0f);
				
			}

			ImGui::End();
		}
	}
}

void ModuleEditor::WindowHierarchy()
{
	if (hierarchy_window)
	{
		
		if (ImGui::Begin("Hierarchy", &hierarchy_window))
		{
			if (App->scene_intro->GetRootObject() != nullptr) {

				HierarchyNodes(App->scene_intro->GetRootObject());

			}
			ImGui::End();
		}
	}
}

void ModuleEditor::HierarchyNodes(const GameObject* node)
{
	std::vector<GameObject*>::const_iterator children = node->children.cbegin();

	const char* rootName = node->GetName();

	ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_None;

	if (node == App->scene_intro->GetRootObject())
	{
		treeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
	}
	if (node->children.empty())
	{
		treeFlags = ImGuiTreeNodeFlags_Leaf;	//Only applyied to the nodes without childrens (Usually the last one)
	}

	if (ImGui::TreeNodeEx(rootName, treeFlags)) {

		for (children; children != node->children.cend(); children++)
		{
			if (!node->children.empty())
			{
				if (ImGui::IsItemClicked())
				{
					
				}

				HierarchyNodes((*children));
			}
		}
		
		ImGui::TreePop();
	}

}

void ModuleEditor::WindowDemo()
{
	if (demo_window)
	{
		//our state (depenent de si el bool que pasem a la window es true o false s'ensenya la window i si la tanquem imgui posa el bool directament a false)
		ImGui::ShowDemoWindow(&demo_window);
	}
}

void ModuleEditor::AddLog(const char* string)
{
	char* new_string = strdup(string);
	string_log.push_back(new_string);

	scroll = true;
}

void ModuleEditor::ClearLog()
{
	for (uint i = 0; i < string_log.size(); ++i)
	{
		free(string_log[i]);
	}

	string_log.clear();

	scroll = true;
}
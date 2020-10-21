#include "Globals.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "imgui.h"
#include "examples\imgui_impl_sdl.h"
#include "examples\imgui_impl_opengl3.h"



ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled),
about_window(false),
config_window(true), options_bool(false), org("CITM"), console_window(true),
active_window(true)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	//PROBA RENDER 1X1 CUBE OPENGL
	static DefaultCube c;
	//c.RenderDirect();
	//c.RenderArrayBuffer();
	c.RenderIndexBuffer();

	//IMGUI!!!!
	/*if (ImGui::IsKeyPressed(SDL_SCANCODE_1)) {
		return UPDATE_STOP;
	}*/
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	//our state (depenent de si el bool que pasem a la window es true o false s'ensenya la window i si la tanquem imgui posa el bool directament a false)
	bool show_demo_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::ShowDemoWindow(&show_demo_window);

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit", "ESC")) {
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Console", NULL, console_window)) {
				console_window = !console_window;
			}
			if (ImGui::MenuItem("Configuration", NULL, config_window)) {
				config_window = !config_window;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("Documentation")) {
				ShellExecuteA(NULL, "open", "https://github.com/M1R4B3L/LegacyEngine", NULL, NULL, 3);
			}
			if (ImGui::MenuItem("Donloawd Latest")) {
				ShellExecuteA(NULL, "open", "https://github.com/M1R4B3L/LegacyEngine/releases", NULL, NULL, 3);
			}
			if (ImGui::MenuItem("Report a bug")) {
				ShellExecuteA(NULL, "open", "https://github.com/M1R4B3L/LegacyEngine/issues", NULL, NULL, 3);
			}
			if (ImGui::MenuItem("About")) {
				about_window = !about_window;
			}
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	if (about_window) {
		//ImGuiCond_Once;
		//ImGui::SetNextWindowPos()
		ImGui::Begin("About", &about_window);
		ImGui::Text("Legacy engine is developed by 2 students from CITM Barcelona \nwithin the context of the game engine subject");
		ImGui::End();
	}

	WindowConfig();

	/*if (console_window)
	{
		ImGui::Begin("Console", &console_window);
	
		for (int i = 0; i < 5; i++) 
		{
			
			ImGui::TextUnformatted("Hi, monkey");
		}
		
		ImGui::End();
	}*/


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

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

bool ModuleSceneIntro::WindowConfig()
{
	if (config_window) {
		//ImGuiCond_Once;
		//ImGui::SetNextWindowPos()
		ImGui::Begin("Configuration", &config_window);
		if (ImGui::BeginMenu("Options", &options_bool)) {
			ImGui::MenuItem("Set Defaults");
			ImGui::MenuItem("Load");
			ImGui::MenuItem("Save");
			ImGui::EndMenu();
		}
		if (ImGui::CollapsingHeader("Application")) {
			
			static char name[128];
			strcpy_s(name, 128, App->GetEngineTitle());
			if(ImGui::InputText("App Name", name, 128, ImGuiInputTextFlags_EnterReturnsTrue))  //We can use ImGuiInputTextFlags_EnterReturnsTrue only if we want to change the name if enter is pressed
				App->SetEngineTitle(name);

			ImGui::InputText("Organization", org, 128);

			ImGui::Text("Limit Framerate:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0,255,255,255), "Aki van unos framesitos");

			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", App->fps_log[App->fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &App->fps_log[0], App->fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			ImGui::Checkbox("Active", &active_window);
			if (active_window == false)
			{
				
			}

			//ImGui::SliderInt("Width", &App->window->GetWidth(),);
			App->window->GetWidth();

		}
		if (ImGui::CollapsingHeader("File System"))
		{

		}
		if (ImGui::CollapsingHeader("Input"))
		{

		}
		if (ImGui::CollapsingHeader("Hardware"))
		{

		}
		if (ImGui::CollapsingHeader("OpenGL"))
		{

		}

		ImGui::End();
	}

	return config_window;
}

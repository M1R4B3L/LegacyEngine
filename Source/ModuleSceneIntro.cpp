#include "Globals.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "imgui.h"
#include "examples\imgui_impl_sdl.h"
#include "examples\imgui_impl_opengl3.h"
#include "SDL_opengl.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled), about_window(false), config_window(false),options_bool(false),name("Legacy Engine"),org("CITM")
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

	
	vertex = {
		//Front
		-1.f, 0.f, 2.f,
		1.f, 0.f, 2.f,
		1.f, 2.f, 2.f,
		-1.f, 0.f, 2.f,
		1.f, 2.f, 2.f,
		-1.f, 2.f, 2.f,
		//Back
		-1.f, 0.f, 0.f,
		-1.f, 2.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		-1.f, 2.f, 0.f,
		1.f, 2.f, 0.f,
		//Right
		1.f, 0.f, 2.f,
		1.f, 2.f, 0.f,
		1.f, 2.f, 2.f,
		1.f, 0.f, 2.f,
		1.f, 0.f, 0.f,
		1.f, 2.f, 0.f,
		//Left
		- 1.f, 0.f, 2.f,
		-1.f, 2.f, 0.f,
		-1.f, 0.f, 0.f,
		-1.f, 0.f, 2.f,
		-1.f, 2.f, 2.f,
		-1.f, 2.f, 0.f,
		//Top
		-1.f, 2.f, 2.f,
		1.f, 2.f, 0.f,
		-1.f, 2.f, 0.f,
		-1.f, 2.f, 2.f,
		1.f, 2.f, 2.f,
		1.f, 2.f, 0.f,
		//Bottom
		1.f, 0.f, 2.f,
		-1.f, 0.f, 2.f,
		-1.f, 0.f, 0.f,
		1.f, 0.f, 2.f,
		-1.f, 0.f, 0.f,
		1.f, 0.f, 0.f
	};

	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, &vertex, GL_STATIC_DRAW);

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
			if (ImGui::Button("Exit")) {
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			if (ImGui::Button("Console")) {
				
			}
			if (ImGui::Button("Configuration")) {
				config_window = !config_window;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::Button("Documentation")) {
				ShellExecuteA(NULL, "open", "https://github.com/M1R4B3L/LegacyEngine", NULL, NULL, 3);
			}
			if (ImGui::Button("Donloawd Latest")) {
				ShellExecuteA(NULL, "open", "https://github.com/M1R4B3L/LegacyEngine/releases", NULL, NULL, 3);
			}
			if (ImGui::Button("Report a bug")) {
				ShellExecuteA(NULL, "open", "https://github.com/M1R4B3L/LegacyEngine/issues", NULL, NULL, 3);
			}
			if (ImGui::Button("About")) {
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
		ImGui::Text("Legacy engine is developed by 2 students from CITM Barcelon \nwithin the context of the game engine subject");
		ImGui::End();
	}

	if (config_window) {
		//ImGuiCond_Once;
		//ImGui::SetNextWindowPos()
		ImGui::Begin("Configuration", &config_window);
		if (ImGui::BeginMenu("Options",&options_bool)) {
			ImGui::MenuItem("Set Defaults");
			ImGui::MenuItem("Load");
			ImGui::MenuItem("Save");
			ImGui::EndMenu();
		}
		if (ImGui::CollapsingHeader("Application")) {
			ImGui::InputText("App Name",name,128);
			ImGui::InputText("Organization", org, 128);
			//ImGui::SliderInt("Max FPS",&App->cap,0,144);
			/*ImGui::Text("Limit Framerate: 0");
			ImGui::GetFrameCount();
			static int n = 0;
			static char title[25];
			if (n > 24)
				n = 0;
			else
				n++;
			title[n]= ImGui::GetFrameCount();
			sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			*/
		}
		ImGui::End();
	}



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

	
	/*glBegin(GL_TRIANGLES);
	//Front
	glVertex3f(-1.f, 0.f, 2.f);
	glVertex3f(1.f, 0.f, 2.f);
	glVertex3f(1.f, 2.f, 2.f);
	glVertex3f(-1.f, 0.f, 2.f);
	glVertex3f(1.f, 2.f, 2.f);
	glVertex3f(-1.f, 2.f, 2.f);

	//Back
	glVertex3f(-1.f, 0.f, 0.f);
	glVertex3f(-1.f, 2.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(-1.f, 2.f, 0.f);
	glVertex3f(1.f, 2.f, 0.f);
	
	//Right
	glVertex3f(1.f, 0.f, 2.f);
	glVertex3f(1.f, 2.f, 0.f);
	glVertex3f(1.f, 2.f, 2.f);
	glVertex3f(1.f, 0.f, 2.f);
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(1.f, 2.f, 0.f);

	//Left
	glVertex3f(-1.f, 0.f, 2.f);
	glVertex3f(-1.f, 2.f, 0.f);
	glVertex3f(-1.f, 0.f, 0.f);
	glVertex3f(-1.f, 0.f, 2.f);
	glVertex3f(-1.f, 2.f, 2.f);
	glVertex3f(-1.f, 2.f, 0.f);

	//Top
	glVertex3f(-1.f, 2.f, 2.f);
	glVertex3f(1.f, 2.f, 0.f);
	glVertex3f(-1.f, 2.f, 0.f);
	glVertex3f(-1.f, 2.f, 2.f);
	glVertex3f(1.f, 2.f, 2.f);
	glVertex3f(1.f, 2.f, 0.f);
	
	//Bottom
	glVertex3f(1.f, 0.f, 2.f);
	glVertex3f(-1.f, 0.f, 2.f); 
	glVertex3f(-1.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, 2.f);
	glVertex3f(-1.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);

	glEnd();*/
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … bind and use other buffers
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}


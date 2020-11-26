#include "Globals.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "SDL_opengl.h"
#include "imgui.h"
#include "examples\imgui_impl_sdl.h"
#include "examples\imgui_impl_opengl3.h"
#include "il.h"
#include "ilu.h"
#include "ilut.h"
#include "ComponentMesh.h"
#include "Shader.h"
#include "physfs.h"
#include "MathGeoLib.h"

ModuleRenderer3D::ModuleRenderer3D(bool startEnable) : Module(startEnable),
wireframes(false)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if (glewInit() != GLEW_OK) {
		LOG("Error initializing glew");
	}

	LOG("OpenGL version: %s", glGetString(GL_VERSION));

	if(ret == true)
	{
		//Use Vsync
		if(SDL_GL_SetSwapInterval(VSYNC) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}
		
		//Grid color
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		//Contains four integer or floating-point values that specify the ambient RGBA reflectance of the material
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
		//Contains four integer or floating-point values that specify the diffuse RGBA reflectance of the material
		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

		//TODO: where do I init physfs library ???
		PHYSFS_init(nullptr);
		if(!PHYSFS_mount("Assets/Shaders/",nullptr,1))
			LOG("Error while setting path: %s\n", PHYSFS_getLastError());
		defaultShader = new Shader("DefaultVertexShader.vs", "DefaultFragmentShader.fs");
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	glDeleteShader(defaultShader->ID); //TODO: maybe on the shader destructor
	RELEASE(defaultShader);
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::Draw(float4x4 modelMatrix, uint VAO, uint indices, uint textureID)
{
	defaultShader->use();
	// draw mesh
	if (textureID != 0) {
		//glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
		//ilutGLBindTexImage();
		//ilutGLBindMipmaps
	}
	defaultShader->setMat4("projection", ProjectionMatrix.M);
	defaultShader->setMat4("view", App->camera->GetViewMatrix());
	defaultShader->setMat4("model", modelMatrix.ptr());
	glBindVertexArray(VAO);
	/*glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);*/

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
	/*glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	//glDisable(GL_TEXTURE_2D);
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

const bool ModuleRenderer3D::GetWireframes()
{
	return wireframes;
}

const bool ModuleRenderer3D::GetglEnableFlags(GLenum flag)
{
	return glIsEnabled(flag);
}

void ModuleRenderer3D::SetWireframes(bool activate)
{
	if (activate != wireframes)
	{
		wireframes = activate;

		if (wireframes == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

void ModuleRenderer3D::SetglEnbleFlags(GLenum flag, bool activate)
{
	if (activate != (bool)glIsEnabled(flag))
	{
		if (activate == true)
		{
			glEnable(flag);
		}
		else
		{
			glDisable(flag);
		}
	}
}

unsigned int ModuleRenderer3D::VAOFromMesh(Mesh mesh)
{
	glGenVertexArrays(1, &mesh.VAO);
	glBindVertexArray(mesh.VAO);

	glGenBuffers(1, &mesh.VBO);
	glGenBuffers(1, &mesh.EBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

	glBufferData(GL_ARRAY_BUFFER, mesh.numVertex * sizeof(float) * 3, mesh.vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.numIndices * sizeof(unsigned int), mesh.index, GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	if (mesh.numTexcoords > 0) {
		uint TexCoordBuffer;
		glGenBuffers(1, &TexCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, TexCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.numTexcoords, mesh.texturecoords, GL_STATIC_DRAW);
		//glTexCoordPointer(2, GL_FLOAT, 0, NULL); //TODO: DEPRECATED: Haig d'escriure els shaders!!!!

		// vertex texture coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	}
	/*
	if (mesh.num_normals > 0) {
		uint NormalsBuffer;
		glGenBuffers(1, &NormalsBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, NormalsBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_normals * 3, mesh.normals, GL_STATIC_DRAW);

		// vertex normals
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	}*/

	glBindVertexArray(0);
	return mesh.VAO;
}

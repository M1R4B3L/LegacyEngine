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
#include "Dependencies/MathGeolib/MathGeoLib.h"
#include "ModuleScene.h"
#include "ComponentCamera.h"
#include "ResourceMesh.h"

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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

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
		lights[0].Active(true);

		//Contains four integer or floating-point values that specify the ambient RGBA reflectance of the material
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
		//Contains four integer or floating-point values that specify the diffuse RGBA reflectance of the material
		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		/*glDisable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);*/

		defaultShader = new Shader("DefaultVertexShader.vs", "DefaultFragmentShader.fs");
		singleColorShader = new Shader("DefaultVertexShader.vs", "SingleColorFragmentShader.fs");
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFrameBuffer);
	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (camera->updatePMatrix)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glLoadMatrixf((GLfloat*)camera->GetGLProjectionMatrix().ptr());

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		camera->updatePMatrix = false;
	}

	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera->GetGLViewMatrix().ptr());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->GetCamera()->frustum.Pos().x, 
					 App->camera->GetCamera()->frustum.Pos().y, 
					 App->camera->GetCamera()->frustum.Pos().z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//App->camera->DrawRay();
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	DeleteBuffers();

	glDeleteShader(defaultShader->ID); //TODO: maybe on the shader destructor
	RELEASE(defaultShader);
	glDeleteShader(singleColorShader->ID); //TODO: maybe on the shader destructor
	RELEASE(singleColorShader);
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::Draw(float4x4 modelMatrix, uint VAO, uint indices, uint textureID, bool outline)
{
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	if (outline) 
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
	}
	else 
	{
		glStencilMask(0x00);
	}

	defaultShader->use();

	if (textureID != 0) {
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	defaultShader->setMat4("projection",App->camera->GetCamera()->GetGLProjectionMatrix().ptr());
	defaultShader->setMat4("view", App->camera->GetCamera()->GetGLViewMatrix().ptr());
	defaultShader->setMat4("model", modelMatrix.ptr());
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (outline) 
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		singleColorShader->use();
		singleColorShader->setMat4("projection", App->camera->GetCamera()->GetGLProjectionMatrix().ptr());
		singleColorShader->setMat4("view", App->camera->GetCamera()->GetGLViewMatrix().ptr());
		float3 translation, scale;
		Quat rotation;
		modelMatrix.Transpose();
		modelMatrix.Decompose(translation, rotation, scale);
		scale.x += outlineWeight; scale.y += outlineWeight; scale.z += outlineWeight;
		modelMatrix = modelMatrix.FromTRS(translation, rotation, scale).Transposed();
		singleColorShader->setMat4("model", modelMatrix.ptr());
		singleColorShader->setFloat4("selectedColor", outlineColor.x, outlineColor.y, outlineColor.z, outlineColor.w);
		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);
	}
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_STENCIL_TEST);
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	//CreateFrameBuffer();
	camera = App->camera->GetCamera();
	camera->SetHoritzontalAspectRatio((float)width / (float)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf((GLfloat*)camera->GetGLProjectionMatrix().ptr());

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

void ModuleRenderer3D::GenMeshBuffers(Mesh* mesh)
{
	glGenVertexArrays(1, &mesh->VAO);
	glBindVertexArray(mesh->VAO);

	glGenBuffers(1, &mesh->VBO);
	glGenBuffers(1, &mesh->EBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

	glBufferData(GL_ARRAY_BUFFER, mesh->numVertices * sizeof(float) * 3, mesh->vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->numIndices * sizeof(unsigned int), mesh->indexData, GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	if (mesh->numTexcoords > 0) {
		glGenBuffers(1, &mesh->textureBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->textureBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->numTexcoords, mesh->texturecoordsData, GL_STATIC_DRAW);
		//glTexCoordPointer(2, GL_FLOAT, 0, NULL);

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
}

void ModuleRenderer3D::DeleteMeshBuffers(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int textureBuffer)
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	if(textureBuffer != 0)
		glDeleteBuffers(1, &textureBuffer);
}

void ModuleRenderer3D::DeleteTexture(unsigned int* texture)
{
	glDeleteTextures(1, texture);
}

void ModuleRenderer3D::DrawAABB(AABB& aabb)
{
	float3 corners[8];
	aabb.GetCornerPoints(corners);
	DrawWireCube(corners, Green);
}

void ModuleRenderer3D::DrawOBB(OBB& obb)
{
	float3 corners[8];
	obb.GetCornerPoints(corners);
	DrawWireCube(corners, Red);
}

void ModuleRenderer3D::DrawFrustum(Frustum& frustum)
{
	float3 corners[8];
	frustum.GetCornerPoints(corners);
	DrawWireCube(corners, Blue);
}

void ModuleRenderer3D::DrawWireCube(float3* vertex, Color color )
{
	glBegin(GL_LINES);

	glColor4f(color.r, color.g, color.b, color.a);

	//Between-planes right
	glVertex3fv((GLfloat*)&vertex[1]);
	glVertex3fv((GLfloat*)&vertex[5]);
	glVertex3fv((GLfloat*)&vertex[7]);
	glVertex3fv((GLfloat*)&vertex[3]);

	//Between-planes left
	glVertex3fv((GLfloat*)&vertex[4]);
	glVertex3fv((GLfloat*)&vertex[0]);
	glVertex3fv((GLfloat*)&vertex[2]);
	glVertex3fv((GLfloat*)&vertex[6]);
						
	//Far plane horizontal 
	glVertex3fv((GLfloat*)&vertex[5]);
	glVertex3fv((GLfloat*)&vertex[4]);
	glVertex3fv((GLfloat*)&vertex[6]);
	glVertex3fv((GLfloat*)&vertex[7]);
						
	//Near plane horizontal
	glVertex3fv((GLfloat*)&vertex[0]);
	glVertex3fv((GLfloat*)&vertex[1]);
	glVertex3fv((GLfloat*)&vertex[3]);
	glVertex3fv((GLfloat*)&vertex[2]);
						
	//Near plane vertical  
	glVertex3fv((GLfloat*)&vertex[1]);
	glVertex3fv((GLfloat*)&vertex[3]);
	glVertex3fv((GLfloat*)&vertex[0]);
	glVertex3fv((GLfloat*)&vertex[2]);
						
	//Far plane vertical   
	glVertex3fv((GLfloat*)&vertex[5]);
	glVertex3fv((GLfloat*)&vertex[7]);
	glVertex3fv((GLfloat*)&vertex[4]);
	glVertex3fv((GLfloat*)&vertex[6]);

	glEnd();
}

void ModuleRenderer3D::DeleteBuffers()
{
	glDeleteFramebuffers(1, &sceneFrameBuffer);
	glDeleteRenderbuffers(1, &sceneTextureBuffer);
	glDeleteFramebuffers(1, &sceneRboDepthStencil);
}

void ModuleRenderer3D::CreateFrameBuffer()
{
	DeleteBuffers();

	glGenFramebuffers(1, &sceneFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFrameBuffer);
	
	//Texture Buffer
	glGenTextures(1, &sceneTextureBuffer);
	glBindTexture(GL_TEXTURE_2D, sceneTextureBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTextureBuffer, 0);
	
	//Object Buffer
	glGenRenderbuffers(1, &sceneRboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, sceneRboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, sceneRboDepthStencil);

	if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG("Error creating frame buffer");
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


#ifndef __ModuleRenderer3D_H__
#define __ModuleRenderer3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "Importer.h"
#include "Math/float4x4.h"

#define MAX_LIGHTS 8

typedef unsigned int GLenum;
class Shader;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool startEnable = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void Draw(float4x4 modelMatrix, uint VAO, uint indices, uint textureId = 0);

	void OnResize(int width, int height);

	bool wireframes;
	const bool GetWireframes();
	const bool GetglEnableFlags(GLenum flag);

	void SetWireframes(bool activate);
	void SetglEnbleFlags(GLenum flag, bool activate);

	unsigned int VAOFromMesh(Mesh mesh);


public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	Shader* defaultShader = nullptr;
	unsigned int dropedTexture = 0;
};

#endif //__ModuleRenderer3D_H__
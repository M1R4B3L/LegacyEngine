#ifndef __ModuleRenderer3D_H__
#define __ModuleRenderer3D_H__

#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "Importer.h"
#include "Dependencies/MathGeolib/MathGeoLib.h"

#define MAX_LIGHTS 8

typedef unsigned int GLenum;
class Shader;
class ComponentCamera;

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

	void GenMeshBuffers(Mesh* mesh);
	void DeleteMeshBuffers(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int textureBuffer);
	void DeleteTexture(unsigned int* texture);

	void DrawAABB(AABB& aabb);
	void DrawOBB(OBB& obb);
	void DrawFrustum(Frustum& frustum);

	void DrawWireCube(float3* vertex, Color color);

	void CreateFrameBuffer();
	void DeleteBuffers();

public:

	//draw scene buffers
	uint sceneFrameBuffer;
	uint sceneTextureBuffer;
	uint sceneRboDepthStencil;

	ComponentCamera* camera = nullptr;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;

	Shader* defaultShader = nullptr;
	unsigned int dropedTexture = 0;
};

#endif //__ModuleRenderer3D_H__
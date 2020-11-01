#pragma once
#ifndef __ModuleCamera3D_H__
#define __ModuleCamera3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void WASDMovement(float dt);
	void WorldTranslation(float dt);
	void WorldRotation(float dt);
	void ReferenceRotation(float dt);
	void Zoom(float dt);

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;

	float move_speed;
	float rotate_speed;
	float zoom_speed;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};

#endif //__ModuleCamera3D_H__
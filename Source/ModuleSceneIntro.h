#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

struct Hardware {

	SDL_version H_SDLVersion;
	int H_CPU;
	int H_CPUCache;
	float H_SystemRAM;
	bool H_rdtsc;
	bool H_3dnow;
	bool H_altivec;
	bool H_avx;
	bool H_avx2;
	bool H_mmx;
	bool H_sse;
	bool H_sse2;
	bool H_sse3;
	bool H_sse41;
	bool H_sse42;

};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/
	
	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	//DefaultCube c;

	//IMGUI!!!!
	bool console_window;
	bool about_window;
	bool config_window;
	bool options_bool;

	char org[128];

	//Config Window
	bool WindowConfig();
	bool active_window;
};

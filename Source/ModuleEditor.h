#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "Globals.h"

enum class ComponentType;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;

struct Hardware {

	SDL_version hardwareSDLVersion;
	int hardwareCPU;
	int hardwareCPUCache;
	float hardwareSystemRAM;
	bool hardwareRdtsc;
	bool hardware3dnow;
	bool hardwareAltivec;
	bool hardwareAvx;
	bool hardwareAvx2;
	bool hardwareMmx;
	bool hardwareSse;
	bool hardwareSse2;
	bool hardwareSse3;
	bool hardwareSse41;
	bool hardwareSse42;

};

class ModuleEditor : public Module
{
public:
	ModuleEditor(bool startEnable = true);
	~ModuleEditor();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	//IMGUI!!!!
	bool dockingWindow;
	bool aboutWindow;
	bool configWindow;
	bool consoleWindow;
	bool inspectorWindow;
	bool hierarchyWindow;
	bool demoWindow;

	char org[128];

	//Config Window

	void WindowDocking();
	void MenuBar();
	void WindowAbout();
	void WindowConfig();
	void WindowConsole();
	void WindowInspector();
	void WindowHierarchy();
	void WindowDemo();

	void AddLog(const char*);
	void ClearLog();

	void HierarchyNodes(GameObject* node);
	void InspectorComponents(GameObject* selectedGameObject);

	void InspectorShowTransform(ComponentTransform* componentTransform);
	void InspectorShowMesh(ComponentMesh* componentMesh);
	void InspectorShowMaterial(ComponentMaterial* componentMaterial);

private:

	std::vector<char*> stringLog;
	bool scroll;
	bool dragDrop;

	GameObject* dragDropObject = nullptr;

};

#endif //__ModuleEditor_H__
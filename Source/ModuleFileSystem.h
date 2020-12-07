#ifndef __ModuleFileSystem_H__
#define __ModuleFileSystem_H__

#include "Module.h"
#include "Globals.h"

class ModuleFileSystem: public Module
{
public:
	ModuleFileSystem(bool startEnable = true);
	~ModuleFileSystem();

	bool Init();
	update_status Update(float dt);

};


#endif //__ModuleFileSystem_H__
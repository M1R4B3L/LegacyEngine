#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

ComponentMaterial::ComponentMaterial():Component(ComponentType::Material),textureID(0)
{
}

ComponentMaterial::ComponentMaterial(unsigned int texture): Component(ComponentType::Material), textureID(texture) {}

ComponentMaterial::~ComponentMaterial() { App->renderer3D->DeleteTexture(&textureID); }

const unsigned int ComponentMaterial::GetID() const
{
	return textureID;
}

void ComponentMaterial::Save(JSON_Array* componentsArry) const
{
	json_array_append_value(componentsArry, json_value_init_object());
	JSON_Object* jsonCMa = json_array_get_object(componentsArry, json_array_get_count(componentsArry) - 1);
	json_object_set_number(jsonCMa, "Type", (int)ComponentType::Material);
	//TODO:FALTA EL RESOURCE MANAGER PER FER EL SAVE DEL UID
	//json_object_set_number(jsonCMa, "ResourceUID", resourceUID);
}

void ComponentMaterial::Load(JSON_Object* componentObj)
{
	//TODO:FALTA EL RESOURCE MANAGER PER FER EL GET DEL UID
	//filesystem/Resources->json_object_get_number(componentObj, "ResourceUID");
}

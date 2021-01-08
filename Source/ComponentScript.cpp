#include "ComponentScript.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ResourceScript.h"

/*ComponentScript::ComponentScript() : Component(ComponentType::Script), resourceID(0), resource(nullptr)
{
}*/

ComponentScript::ComponentScript(unsigned int resourceID) : Component(ComponentType::Script), resourceID(0), resource((ResourceScript*)App->resources->RequestResource(resourceID,Resource::Type::SCRIPT))
{
}

ComponentScript::~ComponentScript()
{
	if (resourceID != 0)
		App->resources->UnrequestResource(resourceID);
}

const unsigned int ComponentScript::GetID() const
{
	return resourceID;
}

const ResourceScript* ComponentScript::GetResource() const
{
	return resource;
}

void ComponentScript::Start()
{
	if (resource->functions.Start != NULL)
		resource->functions.Start(&GetGameObject());
}

void ComponentScript::Update(float dt)
{
	if (resource->functions.Update != NULL)
		resource->functions.Update(&GetGameObject());
}

const char* ComponentScript::GetName()
{
	if (resource->functions.GetScriptName != NULL)
		return resource->functions.GetScriptName();
	else {
		std::string name = "script" + std::to_string(resourceID);
		return name.c_str();
	}
}

void ComponentScript::Save(JSON_Array* componentsArry) const
{
	json_array_append_value(componentsArry, json_value_init_object());
	JSON_Object* jsonCSc = json_array_get_object(componentsArry, json_array_get_count(componentsArry) - 1);
	json_object_set_number(jsonCSc, "Type", (int)ComponentType::Script);
	json_object_set_number(jsonCSc, "ResourceUID", resourceID);
}

void ComponentScript::Load(JSON_Object* componentObj)
{
	resourceID = json_object_get_number(componentObj, "ResourceUID");
	if (resourceID != 0)
		resource = (ResourceScript*)App->resources->RequestResource(resourceID, Resource::Type::SCRIPT);
}

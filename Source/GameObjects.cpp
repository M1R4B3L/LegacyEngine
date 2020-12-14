#include "GameObjects.h"
#include "Component.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Globals.h"
#include "Dependencies/MathGeolib/MathGeoLib.h"
#include "ModuleScene.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"


GameObject::GameObject() : parent(nullptr), name("No name"), uid(App->scene->GetRandomInt())
{

}

GameObject::GameObject(GameObject* iParent, unsigned int UID, const char* iName, float3 transf, float3 scale, Quat rot) : parent(iParent), name(iName), uid(UID)
{
	if (parent)
		parent->children.push_back(this);
}

GameObject::GameObject(GameObject * iParent, const char* iName, float3 transf, float3 scale, Quat rot) : parent(iParent), name(iName), uid(App->scene->GetRandomInt())
{
	if (parent) 
		parent->children.push_back(this);
}

GameObject::~GameObject()
{
	std::vector<Component*>::iterator component = components.begin();
	for (component;component!= components.end();component++)	
		delete (*component);
	components.clear();
}

void GameObject::AddComponent(Component* c)
{
	//TODO:Check if we already have one of this type
	c->SetGameObject(*this);	
	components.emplace_back(c);
}

bool GameObject::RemoveComponent(Component* c)
{
	bool succes = false;
	c->RemoveGameObjectComponent(*this, c->GetType());
	return succes;
}

/*void GameObject::Start()
{
	std::vector<Component*>::iterator it = components.begin();
	for (it; it != components.end(); it++)
		(*it)->Start();
}*/

void GameObject::Update(float dt)
{

	std::vector<Component*>::iterator it = components.begin();
	for (it; it != components.end(); it++)
	{
		(*it)->Update(dt);
	}

	std::vector<GameObject*>::iterator itr = children.begin();
	for (itr; itr != children.end(); itr++) 
	{
		if ((*itr)->activeGameObject)
		{
			(*itr)->Update(dt);

			if (App->renderer3D->camera->ContainsAABB((*itr)->aabb) == true)
			{
				(*itr)->Draw();
			}
		}
	}

	if (HasComponent(ComponentType::Transform))
	{
		((ComponentTransform*)GetComponent(ComponentType::Transform))->GetGlobalTransform();
	}

	if (HasComponent(ComponentType::Camera))
	{
		((ComponentCamera*)GetComponent(ComponentType::Camera))->FrustumUpdateTransform(((ComponentTransform*)GetComponent(ComponentType::Transform))->GetGlobalTransform());
	}

	GenerateAABB();

}

void GameObject::Draw()
{
	const ComponentTransform* transformComponent = (ComponentTransform*)GetComponent(ComponentType::Transform);
	const ComponentMesh* meshComponent = (ComponentMesh*)GetComponent(ComponentType::Mesh);
	ComponentCamera* cameraComponent = (ComponentCamera*)GetComponent(ComponentType::Camera);

	if (meshComponent && transformComponent)
	{
		ResourceMesh* meshResource = (ResourceMesh*)App->resources->GetResource(meshComponent->GetID());

		const ComponentMaterial* materialComponent = (ComponentMaterial*)GetComponent(ComponentType::Material);
		unsigned int material = 0;
		if (materialComponent) 
		{
			ResourceTexture* textureResource = (ResourceTexture*)App->resources->GetResource(materialComponent->GetID());
			material = textureResource->gpuID;
		}
		
		App->renderer3D->Draw(transformComponent->GetGlobalTransform().Transposed(), meshResource->VAOID, meshResource->numIndices, material);

		if (showAABB)
		{
			App->renderer3D->DrawAABB(aabb);
		}

		if (showOBB)
		{
			App->renderer3D->DrawOBB(obb);
		}

	}

	if (cameraComponent)
	{
		App->renderer3D->DrawFrustum(cameraComponent->frustum);
	}
}
 
GameObject* GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetParent(GameObject* newParent)
{
	parent = newParent;
}

bool GameObject::IsActive() const
{
	if(parent != nullptr)
	{
		return parent->IsActive();
	}

	if (activeGameObject == false)
		return false;

	return activeGameObject;
}

const Component* GameObject::GetComponent(ComponentType type) const
{
	std::vector<Component*>::const_iterator it = components.cbegin();
	for (it; it != components.cend(); ++it) {
		if ((*it)->GetType() == type)
			return (*it);
	}

	return nullptr;
}

bool GameObject::HasComponent(ComponentType type) const
{
	std::vector<Component*>::const_iterator it = components.cbegin();
	for (it; it != components.cend(); ++it) {
		if ((*it)->GetType() == type)
			return true;
	}

	return false;
}

std::vector<GameObject*> GameObject::GetChildren() const
{
	return children;
}

const char* GameObject::GetName() const
{
	return name.c_str();
}

void GameObject::SetName(char* newName)
{
	name = newName;
}

void GameObject::GenerateAABB()
{
	ComponentMesh* mesh = ((ComponentMesh*)GetComponent(ComponentType::Mesh));

	if (mesh)
	{
		ResourceMesh* meshResource = (ResourceMesh*)App->resources->GetResource(mesh->GetID());
		obb = meshResource->aabb;
		obb.Transform(((ComponentTransform*)GetComponent(ComponentType::Transform))->GetGlobalTransform());

		aabb.SetNegativeInfinity();
		aabb.Enclose(obb);
	}
}

void GameObject::Save(JSON_Array* GOsarry) const
{
	json_array_append_value(GOsarry, json_value_init_object());
	JSON_Object* jsonGO = json_array_get_object(GOsarry, json_array_get_count(GOsarry) - 1);
	json_object_set_number(jsonGO, "UID", uid);
	//TODO: workaround to avoid the if?
	GameObject* parent = GetParent();
	if(parent != nullptr)
		json_object_set_number(jsonGO, "parentUID", parent->GetID());
	else
		json_object_set_number(jsonGO, "parentUID", 0);
	json_object_set_string(jsonGO, "Name", name.c_str());
	json_object_set_value(jsonGO, "Components", json_value_init_array());
	JSON_Array* componentsArry = json_object_get_array(jsonGO, "Components");

	for (std::vector<Component*>::const_iterator cit = components.cbegin(); cit != components.cend(); ++cit)
		(*cit)->Save(componentsArry);

	for (std::vector<GameObject*>::const_iterator cit = children.cbegin(); cit != children.cend(); ++cit)
		(*cit)->Save(GOsarry);
}

unsigned int GameObject::GetID() const
{
	return uid;
}

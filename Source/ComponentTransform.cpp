#include "ComponentTransform.h"
#include "GameObjects.h"
#include "Globals.h"
#include "Application.h"
#include "scene.h"

ComponentTransform::ComponentTransform(GameObject* go) : Component(go,ComponentType::Transform), translation(float3::zero), scale(float3::one), rotation(Quat::identity),globalFlag(false) 
{ 
	localTransform.SetIdentity();
	globalTransform.SetIdentity();
}

ComponentTransform::ComponentTransform(GameObject* go, float3 iTranslate, float3 iScale, Quat iRotation) : Component(go, ComponentType::Transform), 
translation(iTranslate), scale(iScale), rotation(iRotation), globalFlag(true)
{
	localTransform.SetIdentity();
	globalTransform.SetIdentity();
	localTransform.Set(float4x4::FromTRS(translation, rotation, scale));
	globalTransform.Set(localTransform);
}

ComponentTransform::~ComponentTransform(){}

void ComponentTransform::SetLocalTransform(float3 iTranslate, float3 iScale, Quat iRotation)
{
	localTransform.Set(float4x4::FromTRS(iTranslate,  iRotation, iScale));
	translation = iTranslate;
	scale = iScale;
	rotation = iRotation;
	globalFlag = true;
	/*for (int i = 0; i < GetGameObject().children.size(); ++i) {
		if (GetGameObject().children[i]->HasComponent(ComponentType::Transform))
		{
			((ComponentTransform*)GetGameObject().children[i]->GetComponent(ComponentType::Transform))->SetFlag();
		}
	}*/
}

void ComponentTransform::NewParentLocal(GameObject* newParent)
{
	localTransform.Set(((ComponentTransform*)newParent->GetComponent(ComponentType::Transform))->GetGlobalTransform().Transposed() * globalTransform);
}

void ComponentTransform::SetGlobalTransform()
{
	/*const GameObject* parentGo = GetGameObject().GetParent();
	if ((parentGo->GetParent() == nullptr))
		globalTransform.Set(localTransform);
	else if (parentGo && (parentGo->HasComponent(ComponentType::Transform))) {
		globalTransform.Set(((ComponentTransform*)parentGo->GetComponent(ComponentType::Transform))->GetGlobalTransform() * localTransform);
	}*/
	globalFlag = false;
	ComponentTransform* parentTransform = (ComponentTransform*)GetGameObject().GetParent()->GetComponent(ComponentType::Transform);
	if (!parentTransform)
		globalTransform.Set(localTransform);
	else
		globalTransform.Set(parentTransform->GetGlobalTransform() * localTransform);
	for (int i = 0; i < GetGameObject().children.size(); ++i)
	{
		((ComponentTransform*)GetGameObject().children[i]->GetComponent(ComponentType::Transform))->SetGlobalTransform();
	}

	localTransform.Decompose(translation, rotation, scale);
}

void ComponentTransform::GetLocalTransform(float3& translation, float3& scale, Quat& rotation) const
{
	translation = this->translation;
	scale = this->scale;
	rotation = this->rotation;
}

void ComponentTransform::SetFlag() const
{
	globalFlag = true;
}

const float4x4 ComponentTransform::GetGlobalTransform()
{
	if(globalFlag)
		SetGlobalTransform();
	return globalTransform;
}

void ComponentTransform::SetGlobalTransform(float4x4 transform)
{
	ComponentTransform* parentTransform = (ComponentTransform*)GetGameObject().GetParent()->GetComponent(ComponentType::Transform);
	if (parentTransform) {
		localTransform = ((ComponentTransform*)GetGameObject().GetParent()->GetComponent(ComponentType::Transform))->GetGlobalTransform()/*.Inverted()*/ * transform;
		globalTransform = transform;
	}
	else {
		localTransform = transform;
		globalTransform = transform;
	}
	//localTransform.Decompose(translation, rotation, scale);
	globalFlag = true;
}

void ComponentTransform::Save(JSON_Array* componentsArry) const
{
	json_array_append_value(componentsArry, json_value_init_object());
	JSON_Object* jsonCT = json_array_get_object(componentsArry, json_array_get_count(componentsArry) - 1);
	json_object_set_number(jsonCT, "Type", (int)ComponentType::Transform);
	float3 translation, scale;
	Quat rotation;
	localTransform.Decompose(translation, rotation, scale);

	json_object_set_value(jsonCT, "Translation", json_value_init_array());
	JSON_Array* currArry = json_object_get_array(jsonCT, "Translation");
	json_array_append_number(currArry, translation.x);
	json_array_append_number(currArry, translation.y);
	json_array_append_number(currArry, translation.z);

	json_object_set_value(jsonCT, "Rotation", json_value_init_array());
	currArry = json_object_get_array(jsonCT, "Rotation");
	json_array_append_number(currArry, rotation.x);
	json_array_append_number(currArry, rotation.y);
	json_array_append_number(currArry, rotation.z);
	json_array_append_number(currArry, rotation.w);

	json_object_set_value(jsonCT, "Scale", json_value_init_array());
	currArry = json_object_get_array(jsonCT, "Scale");
	json_array_append_number(currArry, scale.x);
	json_array_append_number(currArry, scale.y);
	json_array_append_number(currArry, scale.z);
}

void ComponentTransform::Load(JSON_Object* componentObj)
{
	JSON_Array* translationArry = json_object_get_array(componentObj,"Translation");
	float3 translation(json_array_get_number(translationArry, 0), json_array_get_number(translationArry, 1), json_array_get_number(translationArry, 2));
	JSON_Array* rotationArry = json_object_get_array(componentObj, "Rotation");
	Quat rotation(json_array_get_number(rotationArry, 0), json_array_get_number(rotationArry, 1), json_array_get_number(rotationArry, 2), json_array_get_number(rotationArry, 3));
	JSON_Array* scaleArry = json_object_get_array(componentObj, "Scale");
	float3 scale(json_array_get_number(scaleArry, 0), json_array_get_number(scaleArry, 1), json_array_get_number(scaleArry, 2));

	localTransform.Set(float4x4::FromTRS(translation, rotation, scale));
	this->translation = translation;
	this->scale = scale;
	this->rotation = rotation;
	globalFlag = true;
}


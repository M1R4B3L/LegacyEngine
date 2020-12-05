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
translation(iTranslate), scale(iScale), rotation(iRotation), globalFlag(false)
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

void ComponentTransform::SetGlobalTransform() const
{
	/*const GameObject* parentGo = GetGameObject().GetParent();
	if ((parentGo->GetParent() == nullptr))
		globalTransform.Set(localTransform);
	else if (parentGo && (parentGo->HasComponent(ComponentType::Transform))) {
		globalTransform.Set(((ComponentTransform*)parentGo->GetComponent(ComponentType::Transform))->GetGlobalTransform() * localTransform);
	}*/
	globalFlag = false;
	if (!GetGameObject().GetParent()->HasComponent(ComponentType::Transform))
		globalTransform.Set(localTransform);
	else
		globalTransform.Set(((ComponentTransform*)(GetGameObject().GetParent()->GetComponent(ComponentType::Transform)))->GetGlobalTransform() * localTransform);
	for (int i = 0; i < GetGameObject().children.size(); ++i)
	{
		((ComponentTransform*)GetGameObject().children[i]->GetComponent(ComponentType::Transform))->SetGlobalTransform();
	}
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

const float4x4 ComponentTransform::GetGlobalTransform() const
{
	if(globalFlag)
		SetGlobalTransform();
	return globalTransform;
}


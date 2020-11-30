#include "ComponentTransform.h"
#include "GameObjects.h"
#include "Globals.h"
#include "Application.h"
#include "scene.h"

ComponentTransform::ComponentTransform(GameObject* go) : Component(go,ComponentType::Transform, true), translation(float3::zero), scale(float3::one), rotation(Quat::identity),globalFlag(false) 
{ 
	localTransform.SetIdentity();
	globalTransform.SetIdentity();
}

ComponentTransform::ComponentTransform(GameObject* go, float3 iTranslate, float3 iScale, Quat iRotation) : Component(go, ComponentType::Transform, true), 
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
}

void ComponentTransform::SetGlobalTransform() const
{
	const GameObject* parentGo = GetGameObject().GetParent();
	if ((parentGo->GetParent() == nullptr))
		globalTransform.Set(localTransform);
	else if (parentGo && (parentGo->HasComponent(ComponentType::Transform))) {
		globalTransform = ((ComponentTransform*)parentGo->GetComponent(ComponentType::Transform))->GetGlobalTransform() * localTransform;
	}
	globalFlag = false;
}

void ComponentTransform::GetLocalTransform(float3& translation, float3& scale, Quat& rotation) const
{
	translation = this->translation;
	scale = this->scale;
	rotation = this->rotation;
}

const float4x4 ComponentTransform::GetGlobalTransform() const
{
	if(globalFlag)
		SetGlobalTransform();
	return globalTransform;
}


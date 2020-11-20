#include "ComponentTransform.h"
#include "GameObjects.h"

ComponentTransform::ComponentTransform(GameObject* go) : Component(go,ComponentType::Transform, true), translation(float3::zero), scale(float3::one), rotation(Quat::identity),globalFlag(true) 
{ 
	localTransform.SetIdentity();
	globalTransform = localTransform;
}

ComponentTransform::ComponentTransform(GameObject* go, float3 iTranslate, float3 iScale, Quat iRotation) : Component(go, ComponentType::Transform, true), 
translation(iTranslate), scale(iScale), rotation(iRotation), globalFlag(true) 
{
	localTransform = float4x4::FromTRS(translation, rotation, scale);
	globalTransform = localTransform;
}

ComponentTransform::~ComponentTransform(){}

void ComponentTransform::SetLocalTransform(float3 iTranslate, float3 iScale, Quat iRotation)
{
	localTransform = float4x4::FromTRS(iTranslate,  iRotation, iScale);
	translation = iTranslate;
	scale = iScale;
	rotation = iRotation;
	globalFlag = true;
}

void ComponentTransform::SetWorldTransform(float4x4 parentGlobal)
{
		globalTransform = parentGlobal * localTransform;
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
	return globalTransform;
}

const bool ComponentTransform::GetGlobalFlag() const
{
	return globalFlag;
}

#pragma once
#include "Component.h"
#include "MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* go);
	ComponentTransform(GameObject* go, float3 iTranslate, float3 iScale, Quat iRotation);
	~ComponentTransform();

	void SetLocalTransform(float3 iTranslate, float3 iScale, Quat iRotation);
	void SetWorldTransform(float4x4 parentGlobal);
	void GetLocalTransform(float3& translation, float3& scale, Quat& rotation) const;
	const float4x4 GetGlobalTransform() const;
	const bool GetGlobalFlag() const;

private:
	float3 translation;
	float3 scale;
	Quat rotation;
	float4x4 localTransform;
	float4x4 globalTransform;
	bool globalFlag;
};


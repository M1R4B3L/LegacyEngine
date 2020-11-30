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
	void GetLocalTransform(float3& translation, float3& scale, Quat& rotation) const;
	void SetFlag() const;
	const float4x4 GetGlobalTransform() const;

private:
	void SetGlobalTransform() const;

	float3 translation;
	float3 scale;
	Quat rotation;
	float4x4 localTransform;
	mutable float4x4 globalTransform;
	mutable bool globalFlag;
};


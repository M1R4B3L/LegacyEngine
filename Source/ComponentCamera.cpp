#include "ComponentCamera.h"
#include "Dependencies/MathGeolib/MathGeoLib.h"

ComponentCamera::ComponentCamera() : Component (ComponentType::Camera), active(true), culling(false), updatePMatrix(true)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.0f, 200.0f);
	frustum.SetFrame(float3(0, 0, 0), float3(0, 0, 1), float3(0, 1, 0));
	frustum.SetPerspective(1.f, 1.f);
	frustum.GetPlanes(frustumPlanes);
	updatePMatrix = true;
}

ComponentCamera::ComponentCamera(GameObject* gameObject) : Component(gameObject, ComponentType::Camera), active(true), culling(false),updatePMatrix(true)
{
	//Basic Component Camera
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.0f, 200.0f);
	frustum.SetFrame(float3(0, 0, 0), float3(0, 0, 1), float3(0, 1, 0));
	frustum.SetPerspective(1.f, 1.f);
	frustum.GetPlanes(frustumPlanes);
	updatePMatrix = true;
}

ComponentCamera::~ComponentCamera()
{
}

float ComponentCamera::GetNearPlaneDistance() const
{
	return frustum.NearPlaneDistance();
}

float ComponentCamera::GetFarPlaneDistance() const
{
	return frustum.FarPlaneDistance();
}

float ComponentCamera::GetVerticalFOV() const
{
	//Vertical Field of view returned in degrees
	return frustum.VerticalFov() * (180 / pi);
}

float ComponentCamera::GetHoritzontalFOV() const
{
	return frustum.HorizontalFov() * (180 / pi);
}

float ComponentCamera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}

void ComponentCamera::SetNearPlaneDistance(float distance)
{
	if (distance > 0 && distance < GetFarPlaneDistance())
	{
		frustum.SetViewPlaneDistances(distance, GetFarPlaneDistance());
	}

	frustum.GetPlanes(frustumPlanes);
	updatePMatrix = true;
}

void ComponentCamera::SetFarPlaneDistance(float distance)
{
	if (distance > 0 && distance > GetNearPlaneDistance())
	{
		frustum.SetViewPlaneDistances(GetNearPlaneDistance(), distance);
	}

	frustum.GetPlanes(frustumPlanes);
	updatePMatrix = true;
}

void ComponentCamera::SetVerticalFOV(float fov)
{
	fov = (fov / 180) * pi;
	frustum.SetVerticalFovAndAspectRatio(fov, frustum.AspectRatio());

	frustum.GetPlanes(frustumPlanes);
	updatePMatrix = true;
}

void ComponentCamera::SetHoritzontalFOV(float fov)
{
	fov = (fov / 180) * pi;
	frustum.SetHorizontalFovAndAspectRatio(fov, frustum.AspectRatio());

	frustum.GetPlanes(frustumPlanes);
	updatePMatrix = true;
}

void ComponentCamera::SetVerticalAspectRatio(float ratio)
{
	float verticalFOV = frustum.VerticalFov();
	frustum.SetVerticalFovAndAspectRatio(verticalFOV, ratio);

	frustum.GetPlanes(frustumPlanes);
	updatePMatrix = true;
}

void ComponentCamera::SetHoritzontalAspectRatio(float ratio)
{
	float horitzontalFOV = frustum.HorizontalFov();
	frustum.SetHorizontalFovAndAspectRatio(horitzontalFOV, ratio);

	frustum.GetPlanes(frustumPlanes);
	updatePMatrix = true;
}

void ComponentCamera::Look(const float3& position)
{
	//Calculate the vector between final pos, init pos;
	float3 vector = position - frustum.Pos();

	float3x3 matrix = float3x3::LookAt(frustum.Front(), vector.Normalized(), frustum.Up(), float3(0, 1, 0));

	frustum.SetFront(matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(matrix.MulDir(frustum.Up()).Normalized());
	updatePMatrix = true;
}

float4x4 ComponentCamera::GetGLViewMatrix() const
{
	return ((float4x4) frustum.ViewMatrix()).Transposed();
}

float4x4 ComponentCamera::GetGLProjectionMatrix() const
{
	return ((float4x4) frustum.ProjectionMatrix()).Transposed();
}




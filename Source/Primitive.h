#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "Dependencies/MathGeolib/Math/float4x4.h"
#include "Color.h"


enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_PLine,
	Primitive_PPlane,
	Primitive_PCube,
	Primitive_PSphere,
	Primitive_PCylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const float3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	float4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class PCube : public Primitive
{
public :
	PCube();
	PCube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	float3 size;
};

// ============================================
class PSphere : public Primitive
{
public:
	PSphere();
	PSphere(float radius);
	//void InnerRender() const;
public:
	float radius;
};

// ============================================
class PCylinder : public Primitive
{
public:
	PCylinder();
	PCylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class PLine : public Primitive
{
public:
	PLine();
	PLine(float x, float y, float z);
	void InnerRender() const;
public:
	float3 origin;
	float3 destination;
};

// ============================================
class PPlane : public Primitive
{
public:
	PPlane();
	PPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	float3 normal;
	float constant;
};

class DefaultPCube {
public:
	DefaultPCube();
	~DefaultPCube();
	void RenderDirect();
	void RenderArrayBuffer();
	void RenderIndexBuffer();

private:
	uint myArrayId;
	uint myIndexArrayId;
	uint IndexBuffer;
};

#endif //__PRIMITIVE_H__
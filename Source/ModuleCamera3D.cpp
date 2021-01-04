#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "GameObjects.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "SDL_opengl.h"
#include <algorithm>
#include "ResourceMesh.h"

ModuleCamera3D::ModuleCamera3D(bool startEnable) : Module(startEnable), moveSpeed(10),rotateSpeed(0.025),zoomSpeed(10)
{
	cameraMain = new ComponentCamera();

	cameraMain->frustum.SetViewPlaneDistances(0.1f, 10000.0f);
	cameraMain->frustum.SetPerspective(1.0f, 1.0f);
	cameraMain->frustum.SetFrame(float3(100, 100, 100), float3(0, 0, 1), float3(0, 1, 0));
	cameraMain->frustum.GetPlanes(cameraMain->frustumPlanes);

	LookAt(float3(0, 0, 0));

	cameraMain->updatePMatrix = true;
}

ModuleCamera3D::~ModuleCamera3D()
{
	RELEASE(cameraMain);
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

ComponentCamera* ModuleCamera3D::GetCamera() const
{
	return cameraMain;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	// Mouse motion ----------------

	//Mouse Movement
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		DragCamera(dt);
	}

	//Mouse Rotation & Keyboard Movement
	if ((App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE))
	{
		WASDMovement(dt);
		WorldRotation(dt);
	}

	//Mouse Zoom
	if (App->input->GetMouseZ() != 0)
	{
		Zoom(dt);
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			ReferenceRotation(dt);
		}
	}

	if ((App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) && (App->scene->GetSelectedObject() != nullptr))
	{
		Focus(App->scene->GetSelectedObject());
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && !App->input->GetImGuiUseMouse())
	{
		MouseObjectSelection();
	}
	// Recalculate matrix -------------

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3& pos)
{
	reference = pos;

	cameraMain->Look(reference);	
}

// -----------------------------------------------------------------
void ModuleCamera3D::MoveTo(const float3& pos)
{
	float3 dist = pos - cameraMain->frustum.Pos();
	cameraMain->frustum.SetPos(pos);
	reference = cameraMain->frustum.Pos() + dist;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3& pos, float distance)
{
	reference = pos;
	cameraMain->Look(reference);

	if (distance > 0.f)
		cameraMain->frustum.SetPos(reference - cameraMain->frustum.Front() * distance);
}


void ModuleCamera3D::WASDMovement(float dt)
{
	float3 newPos(0,0,0);

	float speed = moveSpeed * dt;

	float3 front = cameraMain->frustum.Front();
	float3 right = cameraMain->frustum.WorldRight();

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		speed = speed * 2;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos.y -= speed;

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos -= front * speed;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += front * speed;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= right * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += right * speed;
	}
	else 
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos.y -= speed;

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos -= front * speed;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += front * speed;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= right * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += right * speed;
	}

	cameraMain->frustum.SetPos(cameraMain->frustum.Pos() + newPos);
	cameraMain->frustum.GetPlanes(cameraMain->frustumPlanes);
}

void ModuleCamera3D::DragCamera(float dt)
{
	float3 newPos(0, 0, 0);
	float3 x_pos(0, 0, 0);
	float3 y_pos(0, 0, 0);

	int x_motion = App->input->GetMouseXMotion();
	int y_motion = App->input->GetMouseYMotion();

	float speed = moveSpeed * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		speed = speed * 2;

		if (x_motion != 0)
		{
			x_pos = -x_motion * cameraMain->frustum.WorldRight() * moveSpeed * dt;
		}

		if (y_motion != 0)
		{
			y_pos = y_motion * cameraMain->frustum.Up() * moveSpeed * dt;
		}
	}
	else
	{
		if (x_motion != 0)
		{
			x_pos = -x_motion * cameraMain->frustum.WorldRight() * speed;
		}

		if (y_motion != 0)
		{
			y_pos = y_motion * cameraMain->frustum.Up() * speed;
		}

	}
	newPos = x_pos + y_pos;


	cameraMain->frustum.SetPos(cameraMain->frustum.Pos() + newPos);
	cameraMain->frustum.GetPlanes(cameraMain->frustumPlanes);
}


void ModuleCamera3D::WorldRotation(float dt)
{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = rotateSpeed * dt;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			Quat q = Quat::RotateY(DeltaX);

			cameraMain->frustum.SetFront(q.Mul(cameraMain->frustum.Front()).Normalized());
			cameraMain->frustum.SetUp(q.Mul(cameraMain->frustum.Up()).Normalized());
			/*X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));*/
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Quat q = Quat::RotateAxisAngle(cameraMain->frustum.WorldRight(), DeltaY);

			vec vecY = q.Mul(cameraMain->frustum.Up().Normalized());

			if (vecY.y > 0.0f)
			{
				cameraMain->frustum.SetUp(vecY);
				cameraMain->frustum.SetFront(q.Mul(cameraMain->frustum.Front()).Normalized());
			}
			/*Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}*/
		}
}


void ModuleCamera3D::ReferenceRotation(float dt)
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float3 focus = cameraMain->frustum.Pos() - reference;

	Quat qy(cameraMain->frustum.Up(), dx * dt);
	Quat qx(cameraMain->frustum.WorldRight(), dy*dt);

	focus = qx.Transform(focus);
	focus = qy.Transform(focus);

	cameraMain->frustum.SetPos(focus + reference);

	LookAt(reference);
}

void ModuleCamera3D::Zoom(float dt)
{
	float distance = reference.Distance(cameraMain->frustum.Pos());
	float3 currentPos = cameraMain->frustum.Pos();
	float3 newPos = currentPos;

	if (App->input->GetMouseZ() > 0)
	{
		newPos = currentPos + cameraMain->frustum.Front() * zoomSpeed *distance* dt;
		//Position -= Z * zoomSpeed * dt;
	}
	else
	{
		newPos = currentPos - cameraMain->frustum.Front() * zoomSpeed * distance * dt;
		//Position += Z * zoomSpeed * dt;
	}

	cameraMain->frustum.SetPos(newPos);
}

void ModuleCamera3D::Focus(GameObject* gameObject)
{
	if (gameObject != nullptr)
	{
		float cameraDistance = Length({ 30.0f, 30.0f, 30.0f });

		ComponentTransform* gameObjectTrasnform = (ComponentTransform*)gameObject->GetComponent(ComponentType::Transform);

		float3 translation, scale;
		Quat rotation;

		gameObjectTrasnform->GetGlobalTransform().Decompose(translation, rotation, scale);

		float3 pos = { translation.x, translation.y, translation.z };

		reference = pos;

		Look(reference, cameraDistance);
	}

}

void ModuleCamera3D::DrawRay()
{
	glColor4f(1, 0, 0, 1);

	//Between-planes right
	GLfloat pointA[3] = { rayPicking.a.x, rayPicking.a.y, rayPicking.a.z };
	GLfloat pointB[3] = { rayPicking.b.x, rayPicking.b.y, rayPicking.b.z };

	glBegin(GL_LINES);
	glVertex3fv(pointA);
	glVertex3fv(pointB);
	glEnd();

	glColor4f(1, 1, 1, 1);
}

bool ModuleCamera3D::SortByDistance(const std::pair<float, GameObject*> pair1, const std::pair<float, GameObject*> pair2)
{
	return pair1.first < pair2.first;
}

void ModuleCamera3D::MouseObjectSelection()
{
	//Cast the ray

	float normalizedX = App->input->GetMouseX() / (float) App->window->GetSize().x;
	float normalizedY = App->input->GetMouseY() / (float) App->window->GetSize().y;

	normalizedX = -1.0 + 2.0 * normalizedX;
	normalizedY = 1.0 - 2.0 * normalizedY;

	rayPicking = cameraMain->frustum.UnProjectLineSegment(normalizedX, normalizedY);

	//Order the go by rayhit on aabb
	GameObject* root = App->scene->GetRootObject();
	std::vector<std::pair<float, GameObject*>> hits;
	AABBHitedObjects(&hits, root, &rayPicking);
	std::sort(hits.begin(), hits.end(), ModuleCamera3D::SortByDistance);

	//look if any of the aabb hited actually hits the object
	bool hit = false;
	std::vector<std::pair<float, GameObject*>>::iterator it = hits.begin();
	for (; it != hits.end(); ++it) {
		if ((*it).second != nullptr) {
			if (TriangleIntersection((*it).second, &rayPicking)) {
				hit = true;
				break;
			}
		}
	}

	if (!hit) {
		App->scene->SetGameObjectUnselected();
	}
}

void ModuleCamera3D::AABBHitedObjects(std::vector<std::pair<float, GameObject*>>* hits, GameObject* go, const LineSegment* ray)
{
	std::vector<GameObject*>::iterator itr = go->children.begin();
	for (itr; itr != go->children.end(); itr++)
	{
		if ((*itr)->activeGameObject)
		{
			if ((*itr)->HasComponent(ComponentType::Transform) && (*itr)->HasComponent(ComponentType::Mesh)) {
				float distance = 0.0f;
				float distanceFar = 0.0f;
				if (ray->Intersects((*itr)->GetAABB(), distance, distanceFar))
				{
					hits->push_back({ distance, (*itr) });
				}
			}
		}
		AABBHitedObjects(hits, (*itr), ray);
	}
}

bool ModuleCamera3D::TriangleIntersection(GameObject* object, const LineSegment* ray)
{
	//From the last check on the aabb we know all the objects have component mesh
	ComponentMesh* meshComponent = (ComponentMesh*)object->GetComponent(ComponentType::Mesh);
	float4x4 objectTransform = ((ComponentTransform*)object->GetComponent(ComponentType::Transform))->GetGlobalTransform();
	unsigned int * indices = meshComponent->GetResource()->indexData;
	float* vertices = meshComponent->GetResource()->vertexData;
	unsigned int numIndices = meshComponent->GetResource()->numIndices;

	for (unsigned int i = 0; i < numIndices; i += 3) 
	{
		unsigned int index1, index2, index3;

		index1 = indices[i] * 3;
		float3 triVertex1(&vertices[index1]);

		index2 = indices[i + 1] * 3;
		float3 triVertex2(&vertices[index2]);

		index3 = indices[i + 2] * 3;
		float3 triVertex3(&vertices[index3]);

		Triangle tri(triVertex1, triVertex2, triVertex3);
		tri.Transform(objectTransform);
		if (ray->Intersects(tri, nullptr, nullptr))
		{
			App->scene->SetGameObjectSelected(object);
			return true;
		}
	}
	return false;
}

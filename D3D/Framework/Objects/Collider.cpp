#include "Framework.h"
#include "Collider.h"

//-----------------------------------------------------------------------------
//ColliderObject
//-----------------------------------------------------------------------------
ColliderObject::ColliderObject()
{
	Init = new Transform();
	World = new Transform();
	Collision = new Collider(World, Init);
}

ColliderObject::~ColliderObject()
{
	SafeDelete(Init);
	SafeDelete(World);
	SafeDelete(Collision);
}

//-----------------------------------------------------------------------------
//Collider
//-----------------------------------------------------------------------------
Collider::Collider(Transform * transform, Transform * init)
	: transform(transform)
	, init(init)
{
	lines[0] = Vector3(-0.5f, -0.5f, -0.5f);//좌하근
	lines[1] = Vector3(-0.5f, +0.5f, -0.5f);//좌상근
	lines[2] = Vector3(+0.5f, -0.5f, -0.5f);//우하근
	lines[3] = Vector3(+0.5f, +0.5f, -0.5f);//우상근

	lines[4] = Vector3(-0.5f, -0.5f, +0.5f);//좌하원
	lines[5] = Vector3(-0.5f, +0.5f, +0.5f);//좌상원
	lines[6] = Vector3(+0.5f, -0.5f, +0.5f);//우하원
	lines[7] = Vector3(+0.5f, +0.5f, +0.5f);//우상원
}

Collider::~Collider()
{
}

void Collider::Update()
{
	
}

void Collider::Render(Color color)
{
	Vector3 dest[8];

	Transform temp;
	temp.World(transform->World());

	if (init != nullptr)
		temp.World(init->World() * transform->World());

	Matrix world = temp.World();

	for (UINT i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&dest[i], &lines[i], &world);

	//Front
	DebugLine::Get()->RenderLine(dest[0], dest[1], color);
	DebugLine::Get()->RenderLine(dest[1], dest[3], color);
	DebugLine::Get()->RenderLine(dest[3], dest[2], color);
	DebugLine::Get()->RenderLine(dest[2], dest[0], color);

	//Rear
	DebugLine::Get()->RenderLine(dest[4], dest[5], color);
	DebugLine::Get()->RenderLine(dest[5], dest[7], color);
	DebugLine::Get()->RenderLine(dest[7], dest[6], color);
	DebugLine::Get()->RenderLine(dest[6], dest[4], color);

	//Side
	DebugLine::Get()->RenderLine(dest[0], dest[4], color);
	DebugLine::Get()->RenderLine(dest[1], dest[5], color);
	DebugLine::Get()->RenderLine(dest[2], dest[6], color);
	DebugLine::Get()->RenderLine(dest[3], dest[7], color);
}

bool Collider::Intersection(Vector3 & position, Vector3 & direction, float * outDistance)
{
	return Intersection(Ray(position, direction), outDistance);
}

bool Collider::Intersection(Ray & ray, float * outDistance)
{
	Transform temp;
	temp.World(transform->World());

	if (init != nullptr)
		temp.World(init->World() * transform->World());

	Matrix world = temp.World();
	Vector3 minPosition, maxPosition;
	D3DXVec3TransformCoord(&minPosition, &lines[0], &world); //minPosition = mul(lines[0], World)
	D3DXVec3TransformCoord(&maxPosition, &lines[7], &world);

	if (fabsf(ray.Direction.x) == 0.0f) ray.Direction.x = 1e-6f;
	if (fabsf(ray.Direction.y) == 0.0f) ray.Direction.y = 1e-6f;
	if (fabsf(ray.Direction.z) == 0.0f) ray.Direction.z = 1e-6f;

	float minValue = 0.0f, maxValue = FLT_MAX;

	//Check X
	if (fabsf(ray.Direction.x) >= 1e-6f)
	{
		float value = 1.0f / ray.Direction.x;
		float minX = (minPosition.x - ray.Position.x) * value; //좌하근 w위치 - 마우스의 w위치 -> 0~1 비율화
		float maxX = (maxPosition.x - ray.Position.x) * value; //좌하근 w위치 - 마우스의 w위치 -> 0~1 비율화

		if (minX > maxX)
		{
			float temp = minX;
			minX = maxX;
			maxX = temp;
		}
		
		minValue = max(minX, minValue); //max([0]정점.x - 마우스.x, 0);
		maxValue = min(maxX, maxValue); //min([7]정점.x - 마우스.x, FLT_MAX);

		if (minValue > maxValue) //->true면 충돌이 안됨X
			return false;
	}

	//Check Y
	if (fabsf(ray.Direction.y) >= 1e-6f)
	{
		float value = 1.0f / ray.Direction.y;
		float minY = (minPosition.y - ray.Position.y) * value; //좌하근 w위치 - 마우스의 w위치 -> 0~1 비율화
		float maxY = (maxPosition.y - ray.Position.y) * value; //좌하근 w위치 - 마우스의 w위치 -> 0~1 비율화

		if (minY > maxY)
		{
			float temp = minY;
			minY = maxY;
			maxY = temp;
		}

		minValue = max(minY, minValue);
		maxValue = min(maxY, maxValue);

		if (minValue > maxValue)
			return false;
	}

	//Check Z
	if (fabsf(ray.Direction.z) >= 1e-6f)
	{
		float value = 1.0f / ray.Direction.z;
		float minZ = (minPosition.z - ray.Position.z) * value; //좌하근 w위치 - 마우스의 w위치 -> 0~1 비율화
		float maxZ = (maxPosition.z - ray.Position.z) * value; //좌하근 w위치 - 마우스의 w위치 -> 0~1 비율화

		if (minZ > maxZ)
		{
			float temp = minZ;
			minZ = maxZ;
			maxZ = temp;
		}

		minValue = max(minZ, minValue);
		maxValue = min(maxZ, maxValue);

		if (minValue > maxValue)
			return false;
	}

	if (outDistance != nullptr)
		*outDistance = minValue;

	return true;
}

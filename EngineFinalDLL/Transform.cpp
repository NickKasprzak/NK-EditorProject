#include "Transform.h"

int TheEngine::GetX(TheEngine::Transform* transform)
{
	return transform->x;
}

int TheEngine::GetY(TheEngine::Transform* transform)
{
	return transform->y;
}

int TheEngine::GetZ(TheEngine::Transform* transform)
{
	return transform->z;
}

void TheEngine::SetX(TheEngine::Transform* transform, int newX)
{
	transform->x = newX;
}

void TheEngine::SetY(TheEngine::Transform* transform, int newY)
{
	transform->y = newY;
}

void TheEngine::SetZ(TheEngine::Transform* transform, int newZ)
{
	transform->z = newZ;
}
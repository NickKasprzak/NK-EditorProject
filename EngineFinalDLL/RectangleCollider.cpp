#include "RectangleCollider.h"
#include "Transform.h"

bool TheEngine::RectangleCollider::CheckCollision(RectangleCollider* other)
{
	TheEngine::Transform* thisTransform = getParent()->getTransform();
	TheEngine::Transform* otherTransform = other->getParent()->getTransform();

	if (thisTransform == nullptr || otherTransform == nullptr) { return false; }

	if (thisTransform->x + (width / 2.0f) < otherTransform->x - (other->width / 2.0f) ||
		thisTransform->x - (width / 2.0f) > otherTransform->x + (other->width / 2.0f))
	{
		return false;
	}

	if (thisTransform->y + (height / 2.0f) < otherTransform->y - (other->height / 2.0f) ||
		thisTransform->y - (height / 2.0f) > otherTransform->y + (other->height / 2.0f))
	{
		return false;
	}

	return true;
}
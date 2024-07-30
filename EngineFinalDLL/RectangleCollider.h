#pragma once
#include "Component.h"

namespace TheEngine
{
	class RectangleCollider : public Component
	{
	public:
		static const int RECT_COLLIDER_ID = 2;

		RectangleCollider()
		{
			width = 0;
			height = 0;
		}

		RectangleCollider(GameObject* parent, float width, float height) : Component(parent)
		{
			this->width = width;
			this->height = height;
		}

		bool CheckCollision(RectangleCollider* other);

		float width, height;
	};
}

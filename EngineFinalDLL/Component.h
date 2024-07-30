#pragma once
#include "GameObject.h"

namespace TheEngine
{
	class Component
	{
	public:
		Component()
		{
			parent = nullptr;
		}

		Component(GameObject* parent) : parent(parent)
		{
		}
	
		GameObject* getParent() { return parent; }
		void setParent(GameObject* parent) { this->parent = parent; }
	protected:
		GameObject* parent;
	};
}
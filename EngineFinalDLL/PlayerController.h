#pragma once
#include "Component.h"

namespace TheEngine
{
	class PlayerController : public Component
	{
	public:
		static const int PLAYER_CONTROLLER_ID = 3;

		PlayerController()
		{
		}

		PlayerController(GameObject* parent, int speed) : Component(parent), speed(speed)
		{

		}
		void Update();

		int speed = 0;
	private:
	};
}
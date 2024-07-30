#pragma once
#include "Component.h"
#include "RectangleRenderer.h"

namespace TheEngine
{
	class ColliderColorChanger : public Component
	{
	public:
		static const int COLOR_CHANGER_ID = 4;

		ColliderColorChanger()
		{
			nRed = 0;
			nGreen = 0;
			nBlue = 0;

			oRed = 0;
			oBlue = 0;
			oGreen = 0;
		}

		ColliderColorChanger(GameObject* parent, int red = 0, int green = 0, int blue = 0) : Component(parent)
		{
			nRed = red;
			nGreen = green;
			nBlue = blue;

			oRed = parent->getRenderer()->red;
			oGreen = parent->getRenderer()->green;
			oBlue = parent->getRenderer()->blue;
		}

		void Update();

		int nRed, nGreen, nBlue;
		int oRed, oGreen, oBlue;
	};
}
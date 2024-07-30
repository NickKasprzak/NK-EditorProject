#pragma once
#include "Component.h"

namespace TheEngine
{
	class Transform : public Component
	{
	public:
		static const int TRANSFORM_ID = 0;

		Transform()
		{
			x = 0;
			y = 0;
			z = 0;
		}

		Transform(GameObject* parent) : Component(parent)
		{
			x = 0;
			y = 0;
			z = 0;
		}

		int x, y, z;
	};

	extern "C" __declspec(dllexport) int GetX(Transform* transformPtr);
	extern "C" __declspec(dllexport) int GetY(Transform * transformPtr);
	extern "C" __declspec(dllexport) int GetZ(Transform * transformPtr);
	extern "C" __declspec(dllexport) void SetX(Transform * transformPtr, int newX);
	extern "C" __declspec(dllexport) void SetY(Transform * transformPtr, int newY);
	extern "C" __declspec(dllexport) void SetZ(Transform * transformPtr, int newZ);
}
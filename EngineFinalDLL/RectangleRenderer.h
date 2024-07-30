#pragma once
#include <SDL2/SDL.h>
#include "Component.h"

namespace TheEngine
{
	class RectangleRenderer : public Component
	{
	public:
		static const int RECT_RENDERER_ID = 1;

		RectangleRenderer()
		{
			width = 0;
			height = 0;
			red = 0;
			green = 0;
			blue = 0;
		}

		RectangleRenderer(GameObject* parent, int width = 10, int height = 10, Uint8 red = 0, Uint8 green = 0, Uint8 blue = 0) : Component(parent)
		{
			this->width = width;
			this->height = height;
			this->red = red;
			this->green = green;
			this->blue = blue;
		}

		void Render(SDL_Renderer* renderer);

		int width, height;
		Uint8 red, blue, green;
	};

	extern "C" __declspec(dllexport) int GetWidth(RectangleRenderer * rendererPtr);
	extern "C" __declspec(dllexport) int GetHeight(RectangleRenderer * rendererPtr);
	extern "C" __declspec(dllexport) int GetRed(RectangleRenderer * rendererPtr);
	extern "C" __declspec(dllexport) int GetGreen(RectangleRenderer * rendererPtr);
	extern "C" __declspec(dllexport) int GetBlue(RectangleRenderer * rendererPtr);

	extern "C" __declspec(dllexport) void SetWidth(RectangleRenderer * rendererPtr, int newWidth);
	extern "C" __declspec(dllexport) void SetHeight(RectangleRenderer * rendererPtr, int newHeight);
	extern "C" __declspec(dllexport) void SetRed(RectangleRenderer * rendererPtr, int newRed);
	extern "C" __declspec(dllexport) void SetGreen(RectangleRenderer * rendererPtr, int newGreen);
	extern "C" __declspec(dllexport) void SetBlue(RectangleRenderer * rendererPtr, int newBlue);
}
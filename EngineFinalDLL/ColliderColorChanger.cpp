#include "ColliderColorChanger.h"
#include "RectangleCollider.h"
#include "RectangleRenderer.h"
#include "Engine.h"

void TheEngine::ColliderColorChanger::Update()
{
	RectangleRenderer* parentRenderer = parent->getRenderer();

	if (parentRenderer == nullptr) { return; }

	for (int i = 0; i < TheEngine::MAX_GAME_OBJECTS; i++)
	{
		if (Engine::getInstance()->gameObjects[i].getCollider() == nullptr || &Engine::getInstance()->gameObjects[i] == parent)
		{
			continue;
		}

		if (parent->getCollider()->CheckCollision(Engine::getInstance()->gameObjects[i].getCollider()))
		{
			parentRenderer->red = nRed;
			parentRenderer->green = nGreen;
			parentRenderer->blue = nBlue;

			return;
		}
	}

	parentRenderer->red = oRed;
	parentRenderer->green = oGreen;
	parentRenderer->blue = oBlue;
}
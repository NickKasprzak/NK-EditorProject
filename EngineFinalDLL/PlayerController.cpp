#include "PlayerController.h"
#include "Transform.h"
#include "Engine.h"

void TheEngine::PlayerController::Update()
{
	TheEngine::Transform* parentTransform = getParent()->getTransform();

	if (parentTransform == nullptr) { return; }

	int inputHorizontal = Engine::GetKey(SDL_SCANCODE_DOWN) - Engine::GetKey(SDL_SCANCODE_UP);
	int inputVertical = Engine::GetKey(SDL_SCANCODE_RIGHT) - Engine::GetKey(SDL_SCANCODE_LEFT);

	parentTransform->x += speed * inputVertical;
	parentTransform->y += speed * inputHorizontal;
}
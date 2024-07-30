#include "GameObject.h"
#include "Transform.h"
#include "RectangleRenderer.h"
#include "RectangleCollider.h"
#include "PlayerController.h"
#include "ColliderColorChanger.h"
#include "Engine.h"

namespace TheEngine
{
	void GameObject::Update(SDL_Renderer* renderer)
	{
	}

	Transform* GameObject::createTransform(float x, float y, float z)
	{
		transform = TheEngine::Engine::getInstance()->transformPool.New(this);

		if (transform == nullptr) { return nullptr; }

		transform->x = x;
		transform->y = y;
		transform->z = z;
		return transform;
	}

	RectangleRenderer* GameObject::createRectangleRenderer(float width, float height, int r, int g, int b)
	{
		renderer = TheEngine::Engine::getInstance()->rendererPool.New(this);

		if (renderer == nullptr) { return nullptr; }

		renderer->width = width;
		renderer->height = height;
		renderer->red = r;
		renderer->green = g;
		renderer->blue = b;
		
		return renderer;
	}

	RectangleCollider* GameObject::createRectangleCollider(float width, float height)
	{
		collider = TheEngine::Engine::getInstance()->colliderPool.New(this);

		if (collider == nullptr) { return nullptr; }

		collider->width = width;
		collider->height = height;

		return collider;
	}

	PlayerController* GameObject::createPlayerController(int speed)
	{
		controller = TheEngine::Engine::getInstance()->controllerPool.New(this);

		if (controller == nullptr) { return nullptr; }

		controller->speed = speed;
		return controller;
	}

	ColliderColorChanger* GameObject::createColorChanger(int r, int g, int b)
	{
		colorChanger = TheEngine::Engine::getInstance()->colorChangerPool.New(this);

		if (colorChanger == nullptr) { return nullptr; }
		else if (renderer == nullptr) { return nullptr; }

		colorChanger->nRed = r;
		colorChanger->nGreen = g;
		colorChanger->nBlue = b;

		colorChanger->oRed = renderer->red;
		colorChanger->oGreen = renderer->green;
		colorChanger->oBlue = renderer->blue;
		return colorChanger;
	}

	void GameObject::removeTransform()
	{
		if (transform == nullptr) { return; };
		transform->setParent(nullptr);
		transform = nullptr;
	}

	void GameObject::removeRenderer()
	{
		if (renderer == nullptr) { return; }
		renderer->setParent(nullptr);
		renderer = nullptr;
	}

	void GameObject::RemoveComponents()
	{
		if (transform != nullptr)
		{
			transform->setParent(nullptr);
			TheEngine::Engine::getInstance()->transformPool.Delete(transform);
			transform = nullptr;
		}

		if (renderer != nullptr)
		{
			renderer->setParent(nullptr);
			TheEngine::Engine::getInstance()->rendererPool.Delete(renderer);
			renderer = nullptr;
		}

		if (collider != nullptr)
		{
			collider->setParent(nullptr);
			TheEngine::Engine::getInstance()->colliderPool.Delete(collider);
			collider = nullptr;
		}

		if (controller != nullptr)
		{
			controller->setParent(nullptr);
			TheEngine::Engine::getInstance()->controllerPool.Delete(controller);
			controller = nullptr;
		}

		if (colorChanger != nullptr)
		{
			colorChanger->setParent(nullptr);
			TheEngine::Engine::getInstance()->colorChangerPool.Delete(colorChanger);
			colorChanger = nullptr;
		}
	}


	// Exported DLL functions

	void* CreateTransform(GameObject* obj)
	{
		return obj->createTransform(0, 0, 0);
	}

	void* CreateRenderer(GameObject* obj)
	{
		return obj->createRectangleRenderer(100, 100, 0, 0, 0);
	}

	void* GetTransform(GameObject* obj)
	{
		return obj->getTransform();
	}

	void* GetRenderer(GameObject* obj)
	{
		return obj->getRenderer();
	}

	void DeleteTransform(GameObject* obj)
	{
		TheEngine::Engine::getInstance()->transformPool.Delete(obj->getTransform());
		obj->removeTransform();
	}

	void DeleteRenderer(GameObject* obj)
	{
		TheEngine::Engine::getInstance()->rendererPool.Delete(obj->getRenderer());
		obj->removeRenderer();
	}
}
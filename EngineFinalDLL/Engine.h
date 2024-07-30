#pragma once
#include <iostream>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include "GameObject.h"

#include "ComponentPool.h"
#include "Transform.h"
#include "RectangleCollider.h"
#include "RectangleRenderer.h"
#include "PlayerController.h"
#include "ColliderColorChanger.h"
#include "StackAllocator.h"

namespace TheEngine
{
	const int MAX_GAME_OBJECTS = 50;
	const int WIN_WIDTH = 640;
	const int WIN_HEIGHT = 480;

	class Engine
	{
	public:
		static Engine* CreateInstance()
		{
			if (instance == nullptr)
			{
				instance = new Engine();
				return instance;
			}

			return nullptr;
		}

		static Engine* getInstance() { return instance; }

		Engine()
		{
			// Engine creation now does the SDL initialization process
			SDL_InitSubSystem(SDL_INIT_VIDEO);
			window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_BORDERLESS);
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			const Uint8* keys = SDL_GetKeyboardState(&numKeys);

			keysLastFrame = new Uint8[numKeys];
			keysThisFrame = new Uint8[numKeys];

			memcpy(keysLastFrame, keys, numKeys);
			memcpy(keysThisFrame, keys, numKeys);

			RegisterCompFuncs();
		}

		~Engine()
		{
			delete keysLastFrame;
			delete keysThisFrame;

			// Now closes out SDL
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}

		static int GetCurrentFrame();
		static float GetDeltaTime();
		static int GetKey(int keycode);
		static int GetKeyDown(int keycode);

		void* GetWindowHandle();

		bool RunGameLoop();

		GameObject* CreateGameObject();
		void DestroyGameObject(GameObject* gameObject);
		void DestroyGameObject(int index);

		void SaveLevel(std::string levelPath);
		void LoadLevel(std::string levelPath);
		void UnloadLevel();

		GameObject gameObjects[MAX_GAME_OBJECTS];
		bool isActive[MAX_GAME_OBJECTS];
		
		ComponentPool<Transform> transformPool;
		ComponentPool<RectangleCollider> colliderPool;
		ComponentPool<RectangleRenderer> rendererPool;
		ComponentPool<PlayerController> controllerPool;
		ComponentPool<ColliderColorChanger> colorChangerPool;
		
		typedef void (*compFunc)(GameObject*, std::string);
		std::unordered_map<int, void(*)(GameObject*, std::string)> compFuncMap;

		void RegisterCompFuncs()
		{
			compFuncMap[TheEngine::Transform::TRANSFORM_ID] = CreateTransform; // 0
			compFuncMap[TheEngine::RectangleRenderer::RECT_RENDERER_ID] = CreateRectangleRenderer; // 1
			compFuncMap[TheEngine::RectangleCollider::RECT_COLLIDER_ID] = CreateRectangleCollider; // 2
			compFuncMap[TheEngine::PlayerController::PLAYER_CONTROLLER_ID] = CreatePlayerController; // 3
			compFuncMap[TheEngine::ColliderColorChanger::COLOR_CHANGER_ID] = CreateColliderColorChanger; // 4
		}

	private:
		static Engine* instance;

		// Renderer stuff pulled into engine class from main
		SDL_Window* window;
		SDL_Renderer* renderer;

		// Lets editor simulate and not simulate components at will
		bool simulateGame = true;

		Uint8* keysLastFrame;
		Uint8* keysThisFrame;
		int numKeys;

		int frame = 0;
		int frameStart = 0;
		float deltaTime;

		static void CreateTransform(GameObject* parent, std::string compData);
		static void CreateRectangleRenderer(GameObject* parent, std::string compData);
		static void CreateRectangleCollider(GameObject* parent, std::string compData);
		static void CreatePlayerController(GameObject* parent, std::string compData);
		static void CreateColliderColorChanger(GameObject* parent, std::string compData);
	};


	// Exported DLL functions to interface C++ stuff to a C# readable format

	// Creates a new Engine instance
	extern "C" __declspec(dllexport) void* CreateEngine();
	 
	// Runs the game loop of the given engine
	extern "C" __declspec(dllexport) void EngineGameLoop(Engine * engine);

	// Gets the handle to the engine's window handle for use in the C# app
	extern "C" __declspec(dllexport) void* GetWindowHandle(Engine * engine);

	// Save a level to the given file path
	extern "C" __declspec(dllexport) void SaveLevel(Engine * engine, const char* path);

	// Load a level from the given file path
	extern "C" __declspec(dllexport) void LoadLevel(Engine* engine, const char* path);

	// Creates and returns a game object
	extern "C" __declspec(dllexport) void* CreateGameObject(Engine * engine);

	// Destroys a game object
	extern "C" __declspec(dllexport) void DestroyGameObject(Engine * engine, GameObject* obj);

	// Gets a game object at a specified index
	extern "C" __declspec(dllexport) void* GetGameObjectByIndex(Engine * engine, int index);
}
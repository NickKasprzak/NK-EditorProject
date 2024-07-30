#pragma once
#include <SDL2/SDL.h>

namespace TheEngine
{
	class Transform;
	class RectangleRenderer;
	class RectangleCollider;
	class PlayerController;
	class ColliderColorChanger;

	class GameObject
	{
	public:
		GameObject()
		{
			transform = nullptr;
			renderer = nullptr;
			collider = nullptr;
			controller = nullptr;
			colorChanger = nullptr;
		}

		~GameObject()
		{
			//RemoveComponents();
		}

		void Update(SDL_Renderer* renderer);

		Transform* createTransform(float x = 0, float y = 0, float z = 0);
		RectangleRenderer* createRectangleRenderer(float width, float height, int r, int g, int b);
		RectangleCollider* createRectangleCollider(float width, float height);
		PlayerController* createPlayerController(int speed);
		ColliderColorChanger* createColorChanger(int r, int g, int b);

		Transform* getTransform() { return transform; }
		RectangleRenderer* getRenderer() { return renderer; }
		RectangleCollider* getCollider() { return collider; }
		PlayerController* getPlayerController() { return controller; }
		ColliderColorChanger* getColorChanger() { return colorChanger; }

		/*
		Functions to detach components from the game object.
		Doesn't delete them, so they will still exist in memory, just unused.
		 
		Fully deleting the component can (and should) be done through their component
		pools through an external process.
		 
		Say, for example, an editor deletes a component attached to a game object.
		In order to reflect that the game object no longer references that component,
		as a game object is still capable of referencing memory that has been marked
		as available through its protected variables, those variables need to be set
		to nullptr as deletion through the component pool doesn't do that for us, nor
		can it really considering how the code has been structured.

		If we were to have something in place to handle it like that, it'd probably
		bloat the code an unneccessary amount since the C# layer can do that stuff
		for the engine in less time.

		That all being said, having a the ability to freely assign and remove any
		component at will without having to directly reference them through storing
		them all in a data structure and forcing certain components (transform and
		renderer) to exist by default would help a ton in reducing the amount of
		functions I would need to export for a future project.

		It also makes managing components in general a less of a headache. Theres
		no fear of accidentally assigning a game object another transform and leaving
		the old instance unused in the pool or something. Theres more inherent flexbility,
		which means theres much less to be super worried about.

		Only issue is getting references to a game object's component isn't a
		O(1) lookup, as the engine would have to run through each of the object's
		component and compare eachs type to the desired type until its either
		found or the end of the structure is reached.

		This works for now though.
		*/
		void removeTransform();
		void removeRenderer();
		void removeCollider() {}
		void removePlayerController() {}
		void removeColorChanger() {}

		void RemoveComponents();

	protected:
		Transform* transform;
		RectangleRenderer* renderer;
		RectangleCollider* collider;
		PlayerController* controller;
		ColliderColorChanger* colorChanger;
	};

	// DLL Exports

	// Calls GameObject's component creation functions
	// Returns a reference to the created component for the C#
	// side to create the component in 
	extern "C" __declspec(dllexport) void* CreateTransform(GameObject* obj);
	extern "C" __declspec(dllexport) void* CreateRenderer(GameObject* obj);
	//extern "C" __declspec(dllexport) void* CreateRectangleCollider();
	//extern "C" __declspec(dllexport) void* CreatePlayerController();
	//extern "C" __declspec(dllexport) void* CreateColorChanger();

	// Calls GameObject's component getter functions
	// Returns a reference to the got component for the C#
	// side to create the component in
	extern "C" __declspec(dllexport) void* GetTransform(GameObject* obj);
	extern "C" __declspec(dllexport) void* GetRenderer(GameObject* obj);

	// Deletes the component attached to the GameObject and
	// calls the GameObject's component remove functions
	extern "C" __declspec(dllexport) void DeleteTransform(GameObject* obj);
	extern "C" __declspec(dllexport) void DeleteRenderer(GameObject * obj);
}
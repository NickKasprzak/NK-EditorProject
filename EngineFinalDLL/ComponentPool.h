#pragma once
#include <cassert>
#include <SDL2/SDL.h>

namespace TheEngine
{
	const int MAX_COMPONENTS = 20;

	/*
	* Template class for storing any
	* given component within a continguous
	* space in memory to allow for faster
	* looping through the game's main loop.
	* 
	* This avoids issues caused by randomly
	* allocating things in memory, resulting
	* in both game objects and components
	* potentially being stored far apart, and
	* are as a result slower to iterate over.
	* 
	* This class also manages a free list,
	* a series of pointers threaded through
	* the array that manages what parts of
	* the list are considered free and open
	* to be allocated through.
	*/
	template <typename T>
	class ComponentPool
	{
	public:
		ComponentPool()
		{
			/*
			* Initialize each union to make
			* use of the nextNode aspect of
			* them and point to the next node
			* in the array, effectively
			* initializing our free list to
			* thread through the entire array.
			*/
			for (int i = 0; i < MAX_COMPONENTS - 1; i++)
			{
				components[i].nextNode = &components[i + 1];
				isActive[i] = false;
			}

			/*
			* Set the last union node to point
			* to nothing as its the end of the
			* array, and therefore the free list
			*/
			components[MAX_COMPONENTS - 1].nextNode = nullptr;
			isActive[MAX_COMPONENTS - 1] = false;

			/*
			* Set the first available node in
			* our free list to the first node
			* in the array as a default.
			*/
			firstAvailable = &components[0];
		}

		void Update()
		{
			for (int i = 0; i < MAX_COMPONENTS; i++)
			{
				if (isActive[i] == true)
				{
					components[i].component.Update();
				}
			}
		}

		void Render(SDL_Renderer* renderer)
		{
			for (int i = 0; i < MAX_COMPONENTS; i++)
			{
				if (isActive[i] == true)
				{
					components[i].component.Render(renderer);
				}
			}
		}

		T* New(GameObject* parent)
		{
			/*
			* Takes the space in memory
			* reserved by the first free
			* memory space in the free
			* list and gives it to the
			* game object for reallocation.
			* 
			* Before this is done, the
			* first available node is
			* reassigned to the next node
			* the first node was initially
			* pointing to, providing a
			* new entry point to the free
			* list.
			* 
			* This also allows for creating
			* components in O(1) time
			* complexity.
			*/
			size_t index = firstAvailable - &components[0];

			if (index == MAX_COMPONENTS - 1)
			{
 				return nullptr;
			}

			isActive[index] = true;

			T* newComponent = &firstAvailable->component;
			newComponent->setParent(parent);
			firstAvailable = firstAvailable->nextNode;

			return newComponent;
		}

		/*
		* Should the pool even be concerned with
		* whats stored in the component's parent
		* game object as far as whether it holds
		* a reference to an active or inactive
		* game object?
		* 
		* Given the scenario that something deletes
		* this component and then has new info
		* assigned to its memory block, the game
		* object that created it initially would
		* still refer to that new block of memory
		* and potentially use another object's
		* component. How do I avoid that happening?
		* Should deletion of a component only be
		* done from the object that creates it?
		* Or should there be a way of accessing
		* the parent object's refernce to the
		* component through some means (like a
		* pointer to the parent game object's
		* reference to the component)?
		* 
		* Would it be too rigid to prevent any
		* given component from being deleted
		* without a direct reference to its
		* status within the parent game object?
		* 
		* Could also just have a cleanup function
		* for each component to wipe its values
		* to their defaults and make sure that
		* its virtual so it has to be implemented
		* for every component type.
		*/

		void Delete(T* component)
		{
			/*
			* Marks the given component
			* for deletion by adding it
			* back into the free list at
			* its front. Its space in memory
			* is now occupied by a pointer
			* to the next available node
			* in memory, which was the old
			* first available free node.
			* 
			* This also allows for deletions
			* to be done in O(1) time
			* complexity.
			*/
			Node* oldFirst = firstAvailable;
			firstAvailable = (Node*)component;
			firstAvailable->nextNode = oldFirst;

			size_t index = firstAvailable - &components[0];
			isActive[index] = false;
		}

	private:
		/*
		* A union that contains
		* data that can either be
		* interpreted as a component
		* or a pointer to the next
		* free node in our array,
		* acting as an extension of
		* the free list.
		* 
		* Ideally, nextNode will
		* always be pointing to the
		* next available node, so
		* this should hopefully
		* form our free list
		*/
		struct Node
		{
			T component;
			Node* nextNode;
		};

		// Stores each component
		Node components[MAX_COMPONENTS];

		// Stores info relating to the usage of each component
		bool isActive[MAX_COMPONENTS];

		/*
		* Stores a pointer to the first
		* node in our free list.
		* 
		* The free list itself doesn't
		* have to run in perfect order
		* through the array, only having
		* to iterate through every free
		* node before arriving at the
		* last node that points to null.
		*/
		Node* firstAvailable;
	};
}
#pragma once
#include <cassert>

namespace TheEngine
{
	class StackAllocator
	{
	public:
		StackAllocator(size_t capacity = 65536)
		{
			// Allocates and initializes stack values
			this->capacity = capacity;
			buffer = new char[capacity];
			head = buffer;
		}

		~StackAllocator()
		{
			delete[] buffer;
		}

		// Allocate memory for the given template type
		template<typename T>
		T* New();

		// Allocate memory for an amount of the given template type
		template<typename T>
		T* New(unsigned int count);

		// Clear the stack completely
		void Clear();

		// Clear the stack to a certain spot
		void PopTo(void* ptr);

	private:
		// Pointer to the start of the stack
		char* buffer;

		// Pointer to the currently allocated
		// end of the stack
		char* head;

		// Indicator for the maximum amount
		// of memory the stack can contain
		size_t capacity;
	};

	// Possible to call any constructor from the
	// template type, not just the default.
	template<typename T>
	T* TheEngine::StackAllocator::New()
	{
		// Get size of templated type
		size_t t_size = sizeof(T);

		// Get current allocation size
		size_t currentAlloc = head - buffer;

		// Check if enough memory is left in the stack
		// to allocate a new instance of type
		if (currentAlloc + t_size > capacity)
		{
			// Return nothing as not enough memory exists
			// on the stack to allocate this new type
			return nullptr;

			// Could also assert instead so the program fails.
		}

		// Otherwise, allocate new space by
		// shifting our head ahead by the
		// size of the type and returning
		// the old head to reference it.
		char* old = head;
		head += t_size;

		// Placement new call just to call
		// the templates constructor
		new (old) T();
		return (T*)old;
	}

	template<typename T>
	T* TheEngine::StackAllocator::New(unsigned int count)
	{
		// Get the size of count instances of type T
		size_t t_size_count = sizeof(T) * count;

		size_t currentAlloc = head - buffer;

		if (currentAlloc + t_size_count > capacity)
		{
			return nullptr;
		}

		char* old = head;
		head += t_size_count;
		return (T*)old;
	}
}
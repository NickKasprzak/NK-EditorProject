#include "StackAllocator.h"

void TheEngine::StackAllocator::Clear()
{
	// Jump back to the start of the stack
	// to allow for reallocation from there
	head = buffer;
}

/*
* I'm guessing markers to previously
* allocated spaces in memory would be done
* externally based on the data being allocated
* through a class that implements it, to which
* we can jump to earlier allocated end markers
* placed between allocated blocks of memory.
* 
* Is there a way to make sure that pop to doesn't
* place the head in the middle of an allocated
* block of memory? Considering that stack allocators
* are supposed to allow for anything to be allocated
* to it, meaning theres no defined data type size
* associated with any given instance of one, jumping
* between defined allocated objects in them to
* direct the head pointer to doesn't seem like
* something that can be guaranteed within this
* function alone.
*/
void TheEngine::StackAllocator::PopTo(void* ptr)
{
	// Check if the pointer exists within the stack first
	if (ptr > head && ptr < buffer) { return; }

	// Jump back to where the pointer is located
	head = (char*)ptr;
}
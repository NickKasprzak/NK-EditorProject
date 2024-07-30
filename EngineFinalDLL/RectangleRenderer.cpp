#include "RectangleRenderer.h"
#include "Transform.h"

void TheEngine::RectangleRenderer::Render(SDL_Renderer* renderer)
{
	Transform* parentTransform = getParent()->getTransform();
	int xPos = 0;
	int yPos = 0;

	if (parentTransform != nullptr)
	{
		xPos = parentTransform->x;
		yPos = parentTransform->y;
	}

	// Create a temporary SDL rectangle to pass
	// the class's rectangle data to the SDL renderer
	// without having to store an SDL_Rect instance
	// in this class
	SDL_Rect tempRect =
	{
		xPos,
		yPos,
		width,
		height
	};

	// Set the renderer's draw color to the rectangle's colors
	SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);

	// Draw the rectangle to the screen through the renderer
	SDL_RenderFillRect(renderer, &tempRect);
}

int TheEngine::GetWidth(RectangleRenderer* rendererPtr)
{
	return rendererPtr->width;
}

int TheEngine::GetHeight(RectangleRenderer* rendererPtr)
{
	return rendererPtr->height;
}

int TheEngine::GetRed(RectangleRenderer* rendererPtr)
{
	return rendererPtr->red;
}

int TheEngine::GetGreen(RectangleRenderer* rendererPtr)
{
	return rendererPtr->green;
}

int TheEngine::GetBlue(RectangleRenderer* rendererPtr)
{
	return rendererPtr->blue;
}

void TheEngine::SetWidth(RectangleRenderer* rendererPtr, int newWidth)
{
	rendererPtr->width = newWidth;
}

void TheEngine::SetHeight(RectangleRenderer* rendererPtr, int newHeight)
{
	rendererPtr->height = newHeight;
}

void TheEngine::SetRed(RectangleRenderer* rendererPtr, int newRed)
{
	rendererPtr->red = newRed;
}

void TheEngine::SetGreen(RectangleRenderer* rendererPtr, int newGreen)
{
	rendererPtr->green = newGreen;
}

void TheEngine::SetBlue(RectangleRenderer* rendererPtr, int newBlue)
{
	rendererPtr->blue = newBlue;
}
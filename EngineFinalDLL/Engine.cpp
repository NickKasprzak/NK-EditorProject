#include "Engine.h"
#include "System.h"

#include <iostream>
#include <fstream>
#include <sstream>

TheEngine::Engine* TheEngine::Engine::instance = 0;

bool TheEngine::Engine::RunGameLoop()
{
    SDL_Event event;

    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return false;
        }
    }

    if (SDL_GetTicks64() - frameStart >= 16)
    {
        memcpy(keysThisFrame, SDL_GetKeyboardState(NULL), numKeys);

        frame++;
        frameStart = SDL_GetTicks64();

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        if (simulateGame)
        {
            controllerPool.Update();
            colorChangerPool.Update();
            rendererPool.Render(renderer);
        }

        SDL_RenderPresent(renderer);

        Uint8* temp = keysThisFrame;
        keysThisFrame = keysLastFrame;
        keysLastFrame = temp;
    }
    return true;
}

int TheEngine::Engine::GetCurrentFrame()
{
	return instance->frame;
}

float TheEngine::Engine::GetDeltaTime()
{
	return instance->deltaTime;
}

int TheEngine::Engine::GetKey(int keycode)
{
	// Check the state of the given keycode in
	// the current keys list.
	if (keycode < instance->numKeys)
	{
		return instance->keysThisFrame[keycode];
	}

	// Return -1 to signify the given keycode doesn't
	// actually exist.
	return -1;
}

int TheEngine::Engine::GetKeyDown(int keycode)
{	
	// Compare the states of the given key between
	// the previous and last frame to check if it
	// was only pressed this frame.
	if (keycode < instance->numKeys)
	{
		return !instance->keysLastFrame[keycode] && instance->keysThisFrame[keycode];
	}

	// Return -1 to signify the given keycode doesn't
	// actually exist.
	return -1;
}

void*  TheEngine::Engine::GetWindowHandle()
{
    SDL_SysWMinfo winInfo;
    SDL_GetWindowWMInfo(window, &winInfo);
    return winInfo.info.win.window;
}

TheEngine::GameObject* TheEngine::Engine::CreateGameObject()
{
    for (int i = 0; i < MAX_GAME_OBJECTS; i++)
    {
        if (isActive[i] == false)
        {
            isActive[i] = true;
            return &gameObjects[i];
        }
    }

    return nullptr;
}

void TheEngine::Engine::DestroyGameObject(GameObject* gameObject)
{
    size_t index = gameObject - &gameObjects[0];
    isActive[index] = false;

    gameObject->RemoveComponents();
}

void TheEngine::Engine::DestroyGameObject(int index)
{
    if (index >= MAX_GAME_OBJECTS || index < 0 || !isActive[index]) { return; }

    isActive[index] = false;

    gameObjects[index].RemoveComponents();
}

void TheEngine::Engine::SaveLevel(std::string levelPath)
{
    std::ofstream levelData;
    levelData.open(levelPath);
    int objNum = 0;

    for (int i = 0; i < MAX_GAME_OBJECTS; i++)
    {
        if (!isActive[i]) { continue; }
        GameObject obj = gameObjects[i];

        std::stringstream objStream;
        objStream << objNum << ":";
        
        if (obj.getTransform() != nullptr)
        {
            Transform* t = obj.getTransform();
            objStream << " " << Transform::TRANSFORM_ID << " [" << t->x << " " << t->y << " " << t->z << "]";
        }

        if (obj.getRenderer() != nullptr)
        {
            RectangleRenderer* r = obj.getRenderer();
            objStream << " " << RectangleRenderer::RECT_RENDERER_ID << " [" << r->width << " " << r->height << " " << (int)r->red << " " << (int)r->green << " " << (int)r->blue << "]";
        }
        
        objStream << "\n";
        objNum++;
        levelData << objStream.str();
    }

    levelData.close();
}

void TheEngine::Engine::LoadLevel(std::string levelPath)
{
    UnloadLevel();

    // Load the level data into a file stream
    std::ifstream levelData;
    levelData.open(levelPath);

    // Loop through the file until the end is reached
    while (levelData.peek() != EOF)
    {
        // Get a game object's data
        std::string currentLine;
        std::getline(levelData, currentLine);
        std::stringstream lineStream(currentLine);

        // Create a new game object and get its unique ID
        GameObject* newObj;
        int objID;
        lineStream >> objID;

        if (objID > MAX_GAME_OBJECTS) { continue; }

        newObj = &gameObjects[objID];
        isActive[objID] = true;

        // Loop through each of this object's component data
        std::string token;
        lineStream >> token;
        lineStream.ignore(100, ' ');

        // Loop through the given object's stringstream
        while (!lineStream.eof())
        {
            std::string compID;
            std::string compData;

            int test;

            // Get the component ID
            if (std::getline(lineStream, token, '['))
            {
                compID = token;
                compID.pop_back();
            }

            // Get the component data
            if (std::getline(lineStream, token, ']'))
            {
                compData = token;
                lineStream.ignore(100, ' ');
            }

            // Call the component creation function if it exists
            if (compFuncMap.find(std::stoi(compID)) != compFuncMap.end())
            {
                compFuncMap[std::stoi(compID)](newObj, compData);
            }
        }
    }
}

void TheEngine::Engine::UnloadLevel()
{
    for (int i = 0; i < MAX_GAME_OBJECTS; i++)
    {
        DestroyGameObject(i);
    }
}

void TheEngine::Engine::CreateTransform(GameObject* parent, std::string compData)
{
    std::stringstream dataStream(compData);
    float nums[3] = { 0, 0, 0 };
    int index = 0;

    while (!dataStream.eof())
    {
        std::string token;

        if (std::getline(dataStream, token, ' '))
        {
            nums[index] = std::stof(token);
            index++;
        }
    }

    Transform* newTransform = parent->createTransform();
    newTransform->x = nums[0];
    newTransform->y = nums[1];
    newTransform->z = nums[2];
}

void TheEngine::Engine::CreateRectangleRenderer(GameObject* parent, std::string compData)
{
    std::stringstream dataStream(compData);
    float nums[5] = { 0, 0, 0, 0, 0 };
    int index = 0;

    while (!dataStream.eof())
    {
        std::string token;

        if (std::getline(dataStream, token, ' '))
        {
            nums[index] = std::stoi(token);
            index++;
        }
    }

    RectangleRenderer* newRenderer = parent->createRectangleRenderer(0, 0, 0, 0, 0);
    newRenderer->width = nums[0];
    newRenderer->height = nums[1];
    newRenderer->red = nums[2];
    newRenderer->green = nums[3];
    newRenderer->blue = nums[4];
}

void TheEngine::Engine::CreateRectangleCollider(GameObject* parent, std::string compData)
{
    std::stringstream dataStream(compData);
    float nums[2] = { 0, 0 };
    int index = 0;

    while (!dataStream.eof())
    {
        std::string token;

        if (std::getline(dataStream, token, ' '))
        {
            nums[index] = std::stoi(token);
            index++;
        }
    }

    RectangleCollider* newCollider = parent->createRectangleCollider(0, 0);
    newCollider->width = nums[0];
    newCollider->height = nums[1];
}

void TheEngine::Engine::CreatePlayerController(GameObject* parent, std::string compData)
{
    std::stringstream dataStream(compData);
    float nums[1] = { 0 };
    int index = 0;

    while (!dataStream.eof())
    {
        std::string token;

        if (std::getline(dataStream, token, ' '))
        {
            nums[index] = std::stoi(token);
            index++;
        }
    }

    PlayerController* newController = parent->createPlayerController(0);
    newController->speed = nums[0];
}

void TheEngine::Engine::CreateColliderColorChanger(GameObject* parent, std::string compData)
{
    std::stringstream dataStream(compData);
    float nums[3] = { 0, 0, 0 };
    int index = 0;

    while (!dataStream.eof())
    {
        std::string token;

        if (std::getline(dataStream, token, ' '))
        {
            nums[index] = std::stoi(token);
            index++;
        }
    }

    ColliderColorChanger* newColorChanger = parent->createColorChanger(0, 0, 0);
    newColorChanger->nRed = nums[0];
    newColorChanger->nGreen = nums[1];
    newColorChanger->nBlue = nums[2];

    if (parent->getCollider())
    {
        newColorChanger->oRed = parent->getRenderer()->red;
        newColorChanger->oGreen = parent->getRenderer()->green;
        newColorChanger->oBlue = parent->getRenderer()->blue;
    }
}

// Exported function definitions
void* TheEngine::CreateEngine()
{
    return Engine::CreateInstance();
}

void TheEngine::EngineGameLoop(Engine * engine)
{
    engine->RunGameLoop();
}

void* TheEngine::GetWindowHandle(Engine * engine)
{
    return engine->GetWindowHandle();
}

void TheEngine::SaveLevel(Engine* engine, const char* path)
{
    engine->SaveLevel(path);
}

void TheEngine::LoadLevel(Engine* engine, const char* path)
{
    engine->LoadLevel(path);
}

void* TheEngine::CreateGameObject(Engine* engine)
{
    return engine->CreateGameObject();
}

void TheEngine::DestroyGameObject(Engine* engine, GameObject* obj)
{
    engine->DestroyGameObject(obj);
}

void* TheEngine::GetGameObjectByIndex(Engine* engine, int index)
{
    if (index > MAX_GAME_OBJECTS) { return nullptr; }
    if (engine->isActive[index] == false) { return nullptr; }

    return &engine->gameObjects[index];
}
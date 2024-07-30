#pragma once

#include "System_Common.h"
#include <SDL2/SDL.h>

#define _CRTDBG_MAP_ALLOC
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/*
* Would it be reasonable to have a C# .NET
* forms app interface the functions needed
* to create and access object info through
* its various UI elements, linking the SDL2
* and actual engine related stuff to a Win32
* window in the forms app?
* 
* We could have both frameworks communicate
* through a central editor class or something,
* passing info between each other, like clicking
* on an object in the SDL window displaying
* object info in the forms PropertyGrid, or
* a create object button in the forms app
* creating a new object to be displayed and used
* in-engine. Stuff like that.
* 
* That said, its worth keeping in mind working
* within an entirely seperate framework may be
* out of the scope of this class, as you aren't
* exactly working within the engine anymore but
* alongside it. Although, it might still be in
* scope for the final as you're working alongside
* the engine to bridge communication between
* two applications. It's not dissimilar to a
* scripting language now that I think about it.
* And its not like using the forms app magically
* makes this process a solved case and a free
* project or anything. You still have to do
* things.
* 
* Still, even if I can't do this for the final
* its still really good to finally have made
* some headway in creating a level editor. Once
* I get the other neccessary parts of the engine
* implemented, I can maybe use this engine to
* experiment with this idea some more, then moving
* to using a 3D engine with OGRE after the other
* neccessary parts of the engine have been added
* and the OGRE and physics-related stuff is abstracted
* behind components. As fun as jumping directly
* into making the editor would be, if creating
* that involves making a library or .DLL, it'd
* probably be a better idea to do the other
* low-level stuff first.
*/

namespace TheEngine
{
	class System_Win32 : public System
	{
    public:
        void Initialize();
        void PreGameStartup();
        void PostGameStartup();
        void Shutdown();

        void DisplayMessageBox(const string& message);
        void LogToErrorFile(const string& message);

        void* CreateWindowInstance(const string& name);

        //void RegisterLoadEngineCallback(void (Engine::*callbackFunc)());
        //static void(Engine::*loadCallbackFunc)();
    private:
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

        _CrtMemState m_MemCheckpointStart;

        FILE* m_CONOUT;
        HANDLE m_GameErrorsHandle = 0;

        HINSTANCE m_WinAppInst;
        WNDCLASSW m_WindClass;
        LPCTSTR m_WindName;
        HWND m_WindInst;

        
	};

    // fromUTF8 taken from https://gist.github.com/xebecnan/6d070c93fb69f40c3673.
    wchar_t* fromUTF8(const char* src,
                      size_t src_length,  /* = 0 */
                      size_t* out_length  /* = NULL */
                     );
}


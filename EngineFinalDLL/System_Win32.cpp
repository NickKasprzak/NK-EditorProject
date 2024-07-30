#include "System_Win32.h"
#include "Engine.h"
#include <iostream>

TheEngine::System* TheEngine::System::instance = 0;

void TheEngine::System_Win32::Initialize()
{
	// Allocate a console. Show a system error if it fails or a console already exists.
	if (AllocConsole() == 0)
	{
		DisplayMessageBox("Couldn't allocate console.");
	}

	// Have console output written to CONOUT
	//m_CONOUT = freopen("CONOUT$", "w", stdout);
	freopen_s(&m_CONOUT, "CONOUT$", "w", stdout);

	// Updates the report mode to report warnings to a supplied file handle
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	// Updates the report file to be that of the target of stdout
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}

void TheEngine::System_Win32::PreGameStartup()
{
	/*
	* Store the current state of the systems memory to
	* check for differences and memory leaks when closing.
	*/
	_CrtMemCheckpoint(&m_MemCheckpointStart);
}

void TheEngine::System_Win32::PostGameStartup()
{
	_CrtMemState memCheckpointEnd;
	_CrtMemState memCheckpointDiff;
	_CrtMemCheckpoint(&memCheckpointEnd);

	/*
	* Check the difference in memory states when the
	* program was first ran and now, placing the
	* differences in memory allocations in the diff
	* checkpoint.
	*
	* If the difference in memory states is signficant,
	* the function returns true and dumps all memory
	* leaks to the output window.
	*
	* Updates the report mode to write output to the
	* error log file instead of the console temporarily.
	*/
	if (_CrtMemDifference(&memCheckpointDiff, &m_MemCheckpointStart, &memCheckpointEnd))
	{
		LogToErrorFile("\nMemory leaks:\n");

		_CrtSetReportFile(_CRT_WARN, m_GameErrorsHandle);
		_CrtMemDumpAllObjectsSince(&m_MemCheckpointStart);

		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
		_CrtMemDumpAllObjectsSince(&m_MemCheckpointStart);

		DisplayMessageBox("Memory leaks detected!");
	}
}

void TheEngine::System_Win32::Shutdown()
{
	fclose(m_CONOUT);
	CloseHandle(m_GameErrorsHandle);
}

void TheEngine::System_Win32::DisplayMessageBox(const TheEngine::string& message)
{
	/*
	* First converts the given string to the LPCTSTR format,
	* then creates a message box using the converted string
	* and assigns its type to be OK
	*
	* Frees the converted message string afterwards as it
	* takes up memory.
	*/
	wchar_t* messageWChar = fromUTF8(message.c_str(), message.size(), NULL);
	MessageBox(NULL, messageWChar, L"A Message Box", MB_OK);
	free(messageWChar);
}

void TheEngine::System_Win32::LogToErrorFile(const TheEngine::string& message)
{
	/*
	* Creates a new text file called GameErrors.txt. The
	* CREATE_ALWAYS flag always forces a new file to be
	* created, overwriting the old one. Saves the return
	* handle of the file so it can be written to.
	*/
	if (m_GameErrorsHandle == 0)
	{
		m_GameErrorsHandle = CreateFileW(L"GameErrors.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	/*
	* Writes the given message to the file opened above.
	*/
	WriteFile(m_GameErrorsHandle, message.c_str(), message.size(), NULL, NULL);
}

TheEngine::System* TheEngine::System::Create()
{
	instance = new System_Win32();
	return instance;
}

void* TheEngine::System_Win32::CreateWindowInstance(const TheEngine::string& name)
{
	// Credit to https://www.braynzarsoft.net/viewtutorial/q16390-02-dx9-creating-a-win32-window for assistance with this
	// Credit to https://learn.microsoft.com/en-us/windows/win32/menurc/using-menus#creating-a-class-menu as well
	// ...and https://zetcode.com/gui/winapi/menus/
	// Would probably be best to move some of this stuff to an initialization function.
	m_WinAppInst = GetModuleHandle(NULL);
	m_WindName = TheEngine::fromUTF8(name.c_str(), name.size(), NULL);

	m_WindClass.style = CS_HREDRAW || CS_VREDRAW;
	m_WindClass.lpfnWndProc = WndProc;
	m_WindClass.cbClsExtra = NULL;
	m_WindClass.cbWndExtra = NULL;
	m_WindClass.hInstance = m_WinAppInst;
	m_WindClass.hIcon = LoadIconW(NULL, IDI_WINLOGO); // Somehow replace with custom icon?
	m_WindClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	m_WindClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	m_WindClass.lpszMenuName = MAKEINTRESOURCE(3);
	m_WindClass.lpszClassName = m_WindName;
	if (RegisterClassW(&m_WindClass) == false)
	{
		DisplayMessageBox("Problem!!!!!!!");
	}

	// temp assignment here
	m_WindInst = CreateWindowW(m_WindName, L"Test", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, m_WinAppInst, NULL);

	if (!m_WindInst)
	{
		DisplayMessageBox("Problem!!!!!!!");
	}

	HMENU testMenu = CreateMenu();
	SetMenu(m_WindInst, testMenu);
	//InsertMenuW(testMenu, -1, MF_BYPOSITION, MF_POPUP, MF_STRING);
	AppendMenuW(testMenu, MF_STRING, 1, L"Create");

	ShowWindow(m_WindInst, 1);
	DrawMenuBar(m_WindInst);
	return m_WindInst;
}

/*
void TheEngine::System_Win32::RegisterLoadEngineCallback(void (Engine::* callbackFunc)())
{
	loadCallbackFunc = callbackFunc;
}
*/

// Process events related to the window
LRESULT CALLBACK TheEngine::System_Win32::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_COMMAND:
			switch LOWORD(wParam)
			{
				case 1:
					/*
					if (loadCallbackFunc != nullptr)
					{
						(TheEngine::Engine::getInstance()->*loadCallbackFunc)();
					}
					*/

					//GameObject* newObj = TheEngine::Engine::getInstance()->CreateGameObject();
					//newObj->createTransform();

					break;

				default:
					break;
			}
			break;

		default:
			break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// fromUTF8 taken from https://gist.github.com/xebecnan/6d070c93fb69f40c3673.
wchar_t* TheEngine::fromUTF8(const char* src,
	size_t src_length,  /* = 0 */
	size_t* out_length  /* = NULL */
)
{
	if (!src)
	{
		return NULL;
	}

	if (src_length == 0) { src_length = strlen(src); }
	int length = MultiByteToWideChar(CP_UTF8, 0, src, src_length, 0, 0);
	wchar_t* output_buffer = (wchar_t*)malloc((length + 1) * sizeof(wchar_t));
	if (output_buffer) {
		MultiByteToWideChar(CP_UTF8, 0, src, src_length, output_buffer, length);
		output_buffer[length] = L'\0';
	}
	if (out_length) { *out_length = length; }
	return output_buffer;
}

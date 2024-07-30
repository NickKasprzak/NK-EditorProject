#pragma once
#include <string>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

namespace TheEngine
{
	typedef std::string string;

	class System
	{
	public:
		static System* Create();
		static void Destroy(System* sys) { delete sys; }
		virtual ~System() {}
		static System* GetInstance() { return instance; }

		virtual void Initialize() = 0;
		virtual void PreGameStartup() = 0;
		virtual void PostGameStartup() = 0;
		virtual void Shutdown() = 0;

		virtual void DisplayMessageBox(const string& message) = 0;
		virtual void LogToErrorFile(const string& message) = 0;

		virtual void* CreateWindowInstance(const string& name) = 0;

	private:
		static System* instance;
	};
}
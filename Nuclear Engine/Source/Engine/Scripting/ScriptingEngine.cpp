#include <Base/BuildConfig.h>
#include <assert.h>

#ifdef NUCLEAR_PLATFORM_WINDOWS_PC
#include <Windows.h>

void* load_library(const wchar_t* _pat)
{
	HMODULE h = ::LoadLibraryW(_pat);
	assert(h != nullptr);
	return (void*)h;
}
void* get_export(void* h, const char* name)
{
	void* f = ::GetProcAddress((HMODULE)h, name);
	assert(f != nullptr);
	return f;
}
#else
void* load_library(const char_t* path)
{
	void* h = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
	assert(h != nullptr);
	return h;
}
void* get_export(void* h, const char* name)
{
	void* f = dlsym(h, name);
	assert(f != nullptr);
	return f;
}
#endif

#include "Engine\Scripting\ScriptingEngine.h"
#include "nethost.h"
#include "coreclr_delegates.h"
#include "hostfxr.h"
#pragma comment (lib, "nethost.lib")

namespace NuclearEngine
{
	namespace Scripting
	{
		// Globals to hold hostfxr exports
		hostfxr_initialize_for_runtime_config_fn init_fptr;
		hostfxr_get_runtime_delegate_fn get_delegate_fptr;
		hostfxr_close_fn close_fptr;

		bool ScriptingEngine::Initialize()
		{
			// Pre-allocate a large buffer for the path to hostfxr
			char_t buffer[MAX_PATH];
			size_t buffer_size = sizeof(buffer) / sizeof(char_t);
			int rc = get_hostfxr_path(buffer, &buffer_size, nullptr);
			if (rc != 0)
				return false;

			// Load hostfxr and get desired exports
			void* lib = load_library(buffer);
			init_fptr = (hostfxr_initialize_for_runtime_config_fn)get_export(lib, "hostfxr_initialize_for_runtime_config");
			get_delegate_fptr = (hostfxr_get_runtime_delegate_fn)get_export(lib, "hostfxr_get_runtime_delegate");
			close_fptr = (hostfxr_close_fn)get_export(lib, "hostfxr_close");

			return (init_fptr && get_delegate_fptr && close_fptr);
		}
		void ScriptingEngine::Shutdown()
		{
		}


	}
}
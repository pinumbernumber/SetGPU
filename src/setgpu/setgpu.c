#include <setgpu.h>
#include <misc.h>

// #include <assert.h>


static bool SETGPU_GLOBAL_was_shared_init_done = false;
static SETGPU_Global_Info_t SETGPU_GLOBAL_global_info;

SETGPU_FUNC bool SETGPU_drm_can_run() {
	return true;
}

// SETGPU_FUNC
void SETGPU_shared_init() {
	if (SETGPU_GLOBAL_was_shared_init_done) return;
	SETGPU_GLOBAL_was_shared_init_done = true;
	char* msg;
	asprintf(&msg, "Hi, thread ID: %lu", GetCurrentThreadId());
	// MessageBoxA(NULL, msg, "", MB_OK);
	free(msg);

	SETGPU_GLOBAL_global_info.dxgi_version = SETGPU_DXGI_VERSION_UNDEFINED;
}

SETGPU_FUNC __cdecl void SETGPU_on_real_dll_loaded(HMODULE real_dll) {
	// assert(sizeof(HMODULE)==4);
	SETGPU_shared_init();
}

SETGPU_FUNC SETGPU_Global_Info_t* SETGPU_get_global_info() {
	return &SETGPU_GLOBAL_global_info;
}

SETGPU_FUNC void SETGPU_say_hi(const char* what) {
	printf("Hi\n");
}

SETGPU_FUNC void SETGPU_show_fatal_error(char* msg) {
	char* full_msg;
	asprintf(&full_msg, "SetGPU encountered a fatal error:\n\n%s\n\nTry updating SetGPU. If you are already using the latest version, please report this problem quoting the exact text of the above error message and the game you were trying to play.\n\nPress OK to exit.", msg);
	MessageBoxA(NULL, full_msg, "SetGPU Error", MB_OK);
	free(full_msg);
	abort();
}

HMODULE SETGPU_dll_module;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		SETGPU_dll_module = hModule;
	}
    return TRUE;
}
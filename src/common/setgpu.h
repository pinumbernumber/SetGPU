#pragma once

#ifdef SETGPU_BUILDING_DLL
#define SETGPU_FUNC	__declspec(dllexport)  
#else
#define SETGPU_FUNC	__declspec(dllimport)  
#endif

#include <stdint.h>
#include <stdbool.h>
#include <Windows.h>

SETGPU_FUNC bool SETGPU_drm_can_run();
// SETGPU_FUNC void SETGPU_init(HMODULE real_dll);
SETGPU_FUNC __cdecl void SETGPU_on_real_dll_loaded(HMODULE real_dll);

#define SETGPU_DXGI_VERSION_UNDEFINED 0
#define SETGPU_DXGI_VERSION_OTHER 1
#define SETGPU_DXGI_VERSION_1_0 2
#define SETGPU_DXGI_VERSION_1_1 3

typedef struct {
	unsigned int dxgi_version;
	void* dxgi_device;
} SETGPU_global_info;

SETGPU_FUNC SETGPU_global_info* SETGPU_get_global_info();
SETGPU_FUNC void SETGPU_show_fatal_error(char* msg);
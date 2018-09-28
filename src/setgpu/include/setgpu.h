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
} SETGPU_Global_Info_t;

SETGPU_FUNC SETGPU_Global_Info_t* SETGPU_get_global_info();
SETGPU_FUNC void SETGPU_show_fatal_error(char* msg);

typedef struct {
	char name[128];
	// bool is_suggested
	// char display_name[128];
	void* ptr;
} SETGPU_Options_Dialog_GPU_t;

typedef struct {
	int selected_gpu_index;
} SETGPU_Options_Dialog_Result_t;

typedef struct {
	SETGPU_Options_Dialog_Result_t* output;
	size_t num_available;
	SETGPU_Options_Dialog_GPU_t* gpus;
	int suggested_gpu_index;
} SETGPU_Options_Dialog_Params_t;

SETGPU_FUNC bool SETGPU_show_options_dialog_blocking(SETGPU_Options_Dialog_Params_t* params);

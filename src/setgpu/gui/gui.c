#include <setgpu.h>
#include <misc.h>

#include "resource1.h"

#if BUILDCPU == X64
#define IS_X64 true
#else
#define IS_X64 false
#endif



typedef struct {

	HWND combo;
	bool setup_done;
	SETGPU_Options_Dialog_Params_t* params;

} TestProcState;

extern HMODULE SETGPU_dll_module;

INT_PTR CALLBACK TestProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    // printf("1\n");
	TestProcState* state;
	// printf("TestProc\n");

	if (Message == WM_INITDIALOG) {
    	printf("WM_INITDIALOG\n");

    	state = malloc(sizeof(TestProcState)); // todo free
    	state->setup_done = false;
    	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)state);

    	state->params = (SETGPU_Options_Dialog_Params_t*) lParam;
    	SetDlgItemText(hwnd, IDD_SETGPU_CHOOSEGPU_INFO_LINE1, "Detected APIs:");
#ifdef IS_X64
        SetDlgItemText(hwnd, IDD_SETGPU_CHOOSEGPU_INFO_LINE2, "64-bit");
#else
    	SetDlgItemText(hwnd, IDD_SETGPU_CHOOSEGPU_INFO_LINE2, "32-bit");
#endif

    	state->combo = GetDlgItem(hwnd, IDD_SETGPU_CHOOSEGPU_COMBO);

    	for (int i = 0; i < state->params->num_available; i++) {
    		SETGPU_Options_Dialog_GPU_t* gpu = state->params->gpus + i;
    		SendMessage(state->combo, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)gpu->name);
    	}

    	SendMessage(state->combo, (UINT)CB_SETCURSEL, (WPARAM)state->params->suggested_gpu_index, (LPARAM)0);
    	state->params->output->selected_gpu_index = state->params->suggested_gpu_index;
    	state->setup_done = true;
    	return TRUE;
	}

    // printf("2\n");

	state = (TestProcState*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
    // printf("3\n");

    if (!state) {
        printf("null STATE\n");
         return FALSE;
    }

    // printf("4\n");

    switch(Message)
    {

        case WM_COMMAND:
            printf("5\n");
        	
        	if (HIWORD(wParam) == CBN_SELCHANGE) {
        		printf("slechange\n");
        		if ((HWND)lParam == state->combo) {
        			printf("cmd combo\n");
        			if (!state->setup_done) return FALSE;
        			printf("cmd combo 2\n");
        			int index = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
        			printf("index: %i\n", index);
        			state->params->output->selected_gpu_index = index;
        			return TRUE;
        		} else {
        			return FALSE;
        		}
        	} else {
	            switch(LOWORD(wParam))
	            {
	                case IDD_SETGPU_CHOOSEGPU_OK:
	                    EndDialog(hwnd, IDD_SETGPU_CHOOSEGPU_OK);
	                break;
	                case IDD_SETGPU_CHOOSEGPU_CANCEL:
	                    EndDialog(hwnd, IDD_SETGPU_CHOOSEGPU_CANCEL);
	                break;
	            }
        	}
        break;

        case WM_CLOSE:
        	EndDialog(hwnd, IDD_SETGPU_CHOOSEGPU_CANCEL);
        	break;

        default:
            return FALSE;
    }
    return FALSE;
}

SETGPU_FUNC bool SETGPU_show_options_dialog_blocking(SETGPU_Options_Dialog_Params_t* params) {

    printf("SETGPU_show_options_dialog_blocking\n");

    int ret = DialogBoxParamA(SETGPU_dll_module, MAKEINTRESOURCE(IDD_SETGPU_CHOOSEGPU_DIALOG), GetActiveWindow(), &TestProc, (uintptr_t)params);

    printf("SETGPU_show_options_dialog_blocking 2\n");

    if (ret == IDD_SETGPU_CHOOSEGPU_CANCEL) return false;





    return true;
    
}
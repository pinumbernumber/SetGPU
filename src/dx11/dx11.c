#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>

#include <common/misc.h>
#include <setgpu.h>

EXTERN_REAL(D3D11CreateDevice)
EXTERN_REAL(D3D11CreateDeviceAndSwapChain)

extern const char REAL_DLL_PATH asm("REAL_DLL_PATH");
extern void* RealDll asm("RealDll");

HRESULT WINAPI Fake_D3D11CreateDevice(
    _In_opt_ IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    _In_reads_opt_( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    _COM_Outptr_opt_ ID3D11Device** ppDevice,
    _Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
    _COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext
) asm ("Fake_D3D11CreateDevice");

HRESULT WINAPI Fake_D3D11CreateDeviceAndSwapChain(
  IDXGIAdapter               *pAdapter,
  D3D_DRIVER_TYPE            DriverType,
  HMODULE                    Software,
  UINT                       Flags,
  const D3D_FEATURE_LEVEL    *pFeatureLevels,
  UINT                       FeatureLevels,
  UINT                       SDKVersion,
  const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
  IDXGISwapChain             **ppSwapChain,
  ID3D11Device               **ppDevice,
  D3D_FEATURE_LEVEL          *pFeatureLevel,
  ID3D11DeviceContext        **ppImmediateContext
) asm ("Fake_D3D11CreateDeviceAndSwapChain");;


#include <initguid.h>
DEFINE_GUID(SetGPU_IID_IDXGIFactory2, 0x50c83a1c, 0xe072, 0x4c48, 0x87,0xb0, 0x36,0x30,0xfa,0x36,0xa6,0xd0);
DEFINE_GUID(SetGPU_IID_IDXGIFactory, 0x7b7166ec, 0x21c7, 0x44ae, 0xb2,0x1a, 0xc9,0xae,0x32,0x1a,0xe3,0x69);

// static const WCHAR* GLOBAL_whitelisted_gpu_name = L"NVIDIA GeForce GTX 960";
// static const char* GLOBAL_whitelisted_gpu_name = "Intel(R) UHD Graphics 630";
// static const char* GLOBAL_whitelisted_gpu_name = "Radeon";
static const char* GLOBAL_whitelisted_gpu_name = "NVIDIA";

bool is_device_name_ok(char* name) {
    printf("%s\n", name);
    return strstr(name, GLOBAL_whitelisted_gpu_name) != NULL;
    // return true;
}

// void* find_correct_adapter() {
//     SETGPU_Global_Info_t* info = SETGPU_get_global_info();
//     bool use_dxgi10 = (info->dxgi_version == SETGPU_DXGI_VERSION_1_0);

//     IDXGIFactory* factory;
//     if (use_dxgi10) {
//         // 1.0
//         printf("Using DXGI 1.0\n");
//         CreateDXGIFactory(&SetGPU_IID_IDXGIFactory, (void**)&factory);
//     } else {
//         printf("Assuming DXGI 1.1\n");
//         CreateDXGIFactory1(&SetGPU_IID_IDXGIFactory2, (void**)&factory);
//     }

//     UINT i = 0;
//     IDXGIAdapter* temp_adapter = NULL;
//     while(1) {
//         bool done = (factory->lpVtbl->EnumAdapters(factory, i, &temp_adapter)) == DXGI_ERROR_NOT_FOUND;
//         if (done) {
//             break;
//         } else {


//             bool is_ok = is_device_name_ok(gpu_name);
//             if (is_ok) {
//                 printf("Using GPU: %s\n", gpu_name);
//                 break;
//             } else {
//                 // continue
//             }
//             i++;
//         }
//     }

//     return temp_adapter;
// }

static void get_gpu_list(SETGPU_Options_Dialog_GPU_t* out_gpus, int* out_num_gpus, int max_gpus) {

    SETGPU_Global_Info_t* info = SETGPU_get_global_info();
    bool use_dxgi10 = (info->dxgi_version == SETGPU_DXGI_VERSION_1_0);

    IDXGIFactory* factory;
    if (use_dxgi10) {
        // 1.0
        printf("Using DXGI 1.0\n");
        CreateDXGIFactory(&SetGPU_IID_IDXGIFactory, (void**)&factory);
    } else {
        printf("Assuming DXGI 1.1\n");
        CreateDXGIFactory1(&SetGPU_IID_IDXGIFactory2, (void**)&factory);
    }


    IDXGIAdapter* temp_adapter = NULL;
    *out_num_gpus = 0;

    for (int i = 0; i < max_gpus; i++) {
        HRESULT res = factory->lpVtbl->EnumAdapters(factory, i, &temp_adapter);
        if (res == DXGI_ERROR_NOT_FOUND) {
            break;
        }
        (*out_num_gpus)++;
        out_gpus[i].ptr = temp_adapter;
        DXGI_ADAPTER_DESC desc;
        temp_adapter->lpVtbl->GetDesc(temp_adapter, &desc);
        // char gpu_name[128];
        wcstombs(out_gpus[i].name, desc.Description, 128);
    }
}

static IDXGIAdapter* _get_adapter() {


    int num_gpus;
    SETGPU_Options_Dialog_GPU_t gpus[SETGPU_MAX_GPUS];

    get_gpu_list(gpus, &num_gpus, SETGPU_MAX_GPUS);

    printf("-----\n");
    for (int i = 0; i < num_gpus; i++) {
        printf("%s\n", gpus[i].name);
    }


    printf("-----\n");


    SETGPU_Options_Dialog_Result_t result;

    SETGPU_Options_Dialog_Params_t params = {
        .output = &result,
        .num_available = num_gpus,
        .gpus = gpus,
        .suggested_gpu_index = 0, 
    };

    bool ret = SETGPU_show_options_dialog_blocking(&params);

    printf("ret %i, idx %i\n", (ret?1:0), result.selected_gpu_index);

    // abort();
    // system("PAUSE");

    // pAdapter = find_correct_adapter();

    IDXGIAdapter* pAdapter = params.gpus[result.selected_gpu_index].ptr;

    return pAdapter;
}

HRESULT WINAPI Fake_D3D11CreateDevice(
    _In_opt_ IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    _In_reads_opt_( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    _COM_Outptr_opt_ ID3D11Device** ppDevice,
    _Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
    _COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext
) {


    pAdapter = _get_adapter();

    HRESULT res = Real_D3D11CreateDevice(
        pAdapter,
        D3D_DRIVER_TYPE_UNKNOWN,
        Software,
        Flags,
        pFeatureLevels,
        FeatureLevels,
        SDKVersion,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext
    );

    return res;
}

HRESULT WINAPI Fake_D3D11CreateDeviceAndSwapChain(
  IDXGIAdapter               *pAdapter,
  D3D_DRIVER_TYPE            DriverType,
  HMODULE                    Software,
  UINT                       Flags,
  const D3D_FEATURE_LEVEL    *pFeatureLevels,
  UINT                       FeatureLevels,
  UINT                       SDKVersion,
  const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
  IDXGISwapChain             **ppSwapChain,
  ID3D11Device               **ppDevice,
  D3D_FEATURE_LEVEL          *pFeatureLevel,
  ID3D11DeviceContext        **ppImmediateContext
) {

    pAdapter = _get_adapter();

    HRESULT res = Real_D3D11CreateDeviceAndSwapChain(
        pAdapter,
        D3D_DRIVER_TYPE_UNKNOWN,
        Software,
        Flags,
        pFeatureLevels,
        FeatureLevels,
        SDKVersion,
        pSwapChainDesc,
        ppSwapChain,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext
    );

    return res;


}
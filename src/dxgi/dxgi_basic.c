#include <Windows.h>
#include <dxgi1_3.h>

#include <common/misc.h>
#include <setgpu.h>


EXTERN_REAL(CreateDXGIFactory)
EXTERN_REAL(CreateDXGIFactory1)
EXTERN_REAL(CreateDXGIFactory2)





HRESULT WINAPI Fake_CreateDXGIFactory(REFIID riid, void **factory) asm("Fake_CreateDXGIFactory");
HRESULT WINAPI Fake_CreateDXGIFactory(REFIID riid, void **factory) {
    SETGPU_get_global_info()->dxgi_version = SETGPU_DXGI_VERSION_1_0;
    HRESULT res = Real_CreateDXGIFactory(riid, factory);
    return res;
}



HRESULT WINAPI Fake_CreateDXGIFactory1(REFIID riid, void **factory) asm("Fake_CreateDXGIFactory1");
HRESULT WINAPI Fake_CreateDXGIFactory1(REFIID riid, void **factory) {
    SETGPU_get_global_info()->dxgi_version = SETGPU_DXGI_VERSION_1_1;
    HRESULT res = Real_CreateDXGIFactory1(riid, factory);
    return res;
}

HRESULT WINAPI Fake_CreateDXGIFactory2(UINT Flags, REFIID riid, void **ppFactory) asm("Fake_CreateDXGIFactory2");
HRESULT WINAPI Fake_CreateDXGIFactory2(UINT Flags, REFIID riid, void **ppFactory) {
    SETGPU_get_global_info()->dxgi_version = SETGPU_DXGI_VERSION_1_1; // hmm
    HRESULT res = Real_CreateDXGIFactory2(Flags, riid, ppFactory);
    return res;
}

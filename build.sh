#! /bin/bash
set -e

# Set architecture as env var e.g.
# BUILDCPU=X64 ./build.sh

# Need bash and related tools (e.g. Git Bash provides this), MinGW, and PHP(!) in your PATH

############################################

# echo Generating definitions...
# node gen_decs.js
# echo Generated.

# echo Compiling+Linking...



if [ -z $NOWIPEBIN ]
then
	rm -rf bin
else
	echo "Not wiping"
fi


mkdir -p bin/dx11/x64
mkdir -p bin/dxgi/x64
mkdir -p bin/setgpu/x64
mkdir -p bin/dx11/x32
mkdir -p bin/dxgi/x32
mkdir -p bin/setgpu/x32

if [ $BUILDCPU = "X64" ]
then
	GCC='gcc'
	CPUFLAGS='-DCPU64 -m64'
	X32_or_X64=x64
elif [ $BUILDCPU = "X32" ]
then
	#-Wl,--kill-at --no-leading-underscore
	GCC='C:\mingw\mingw32\bin\gcc.exe'
	# GCC='tcc -I"C:/Code/winapi-full-for-0.9.27/include" -I"C:/Code/winapi-full-for-0.9.27/include/winapi"'
	CPUFLAGS='-DCPU32 -m32' 
	X32_or_X64=x32
else
	echo "Error: Specify CPU"
	exit 1
fi

# rm -rf bin/dx11/${X32_or_X64}/*.dll
# rm -rf bin/dx11/${X32_or_X64}/*.dll
# rm -rf _obj/dx11/${X32_or_X64}/*

CFLAGS="-Wall -g ${CPUFLAGS} -Isrc -Isrc/common"

# -fPIC

# rm -f bin/dx11/${X32_or_X64}/setgpu.dll 

${GCC} ${CFLAGS} -Isrc/setgpu -DSETGPU_BUILDING_DLL -shared \
-o bin/setgpu/${X32_or_X64}/setgpu.dll \
src/setgpu/setgpu.c

# cp bin/setgpu/${X32_or_X64}/setgpu.dll bin/dx11/${X32_or_X64}/setgpu.dll 

echo Built setgpu.dll

SETGPULFLAGS="-Lbin/setgpu/${X32_or_X64}/ -lsetgpu"

### DX11 ###

rm -f src/dx11/_generated/_d3d11_proxy.S

REAL_DLL_FIXED_PATH=C:\\\\Windows\\\\System32\\\\d3d11.dll \
FAKE_FUNCS=D3D11CreateDevice \
PATH_TO_DEF=`realpath src/dx11/d3d11.def` \
php src/common/dll_proxy_template.php.S > src/dx11/_generated/_d3d11_proxy.S

${GCC} ${CFLAGS} -Isrc/dx11 -shared ${SETGPULFLAGS} \
-o bin/dx11/${X32_or_X64}/d3d11.dll \
src/dx11/_generated/_d3d11_proxy.S \
src/dx11/dx11.c \
src/dx11/d3d11.def \
-ldxgi




rm -f src/dx11/_generated/_dxgi_proxy.S

# CreateDXGIFactory,CreateDXGIFactory1,CreateDXGIFactory2

REAL_DLL_FIXED_PATH=C:\\\\Windows\\\\System32\\\\dxgi.dll \
FAKE_FUNCS=CreateDXGIFactory,CreateDXGIFactory1,CreateDXGIFactory2 \
PATH_TO_DEF=`realpath src/dxgi/dxgi.def` \
php src/common/dll_proxy_template.php.S > src/dxgi/_generated/_dxgi_proxy.S

echo generated dxgi

${GCC} ${CFLAGS} -Isrc/dxgi -shared ${SETGPULFLAGS} \
-o bin/dxgi/${X32_or_X64}/dxgi.dll \
src/dxgi/_generated/_dxgi_proxy.S \
src/dxgi/dxgi_basic.c \
src/dxgi/dxgi.def


echo Built

# cp bin/dx11/${X32_or_X64}/d3d11.dll "D:\do_not_backup\SteamLibrary\steamapps\common\Dishonored2\d3d11.dll"
# cp bin/dx11/${X32_or_X64}/dxgi.dll "D:\do_not_backup\SteamLibrary\steamapps\common\Dishonored2\dxgi.dll"
# cp bin/dx11/${X32_or_X64}/setgpu.dll "D:\do_not_backup\SteamLibrary\steamapps\common\Dishonored2\setgpu.dll"

### END DX11 ###
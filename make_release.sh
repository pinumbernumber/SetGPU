#! /bin/bash
set -e

rm -rf bin
NOWIPEBIN=true BUILDCPU=X64 ./build.sh
NOWIPEBIN=true BUILDCPU=X32 ./build.sh

rm -rf _release_temp/*
mkdir -p _release_temp

X64DIR="_release_temp/For_64-bit_games"
mkdir $X64DIR
cp bin/dx11/x64/d3d11.dll $X64DIR/d3d11.dll
cp bin/dxgi/x64/dxgi.dll $X64DIR/dxgi.dll
cp bin/setgpu/x64/setgpu.dll $X64DIR/setgpu.dll


X32DIR="_release_temp/For_32-bit_games"
mkdir $X32DIR
cp bin/dx11/x32/d3d11.dll $X32DIR/d3d11.dll
cp bin/dxgi/x32/dxgi.dll $X32DIR/dxgi.dll
cp bin/setgpu/x32/setgpu.dll $X32DIR/setgpu.dll

cp misc/!!README!!.txt _release_temp/

if [ -z $DEV ]
then
	echo "not dev"
else
	cp test_hosts/x64/* $X64DIR
	cp test_hosts/x32/* $X32DIR
fi

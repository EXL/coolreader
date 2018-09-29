#!/bin/bash
# Build App for MS Windows, Qt 5, x86_64

cd "$(dirname "$0")"

pacman -Syuu --noconfirm --force
pacman --noconfirm -S --needed mingw-w64-x86_64-toolchain
pacman --noconfirm -S --needed mingw-w64-x86_64-curl
pacman --noconfirm -S --needed mingw-w64-x86_64-cmake
pacman --noconfirm -S --needed mingw-w64-x86_64-qt5-static
pacman --noconfirm -S --needed mingw-w64-x86_64-libpng
pacman --noconfirm -S --needed mingw-w64-x86_64-jasper

cat /proc/cpuinfo

cd ../
mkdir build/
cd build/

cmake -G"MSYS Makefiles" -DCMAKE_PREFIX_PATH=$MINGW_PREFIX/qt5-static/ -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS:STRING="-static -static-libgcc -static-libstdc++" -DQT_QMAKE_EXECUTABLE=$MINGW_PREFIX/qt5-static/bin/qmake.exe -DUSE_QT_PNG=1 -DCMAKE_CXX_FLAGS:STRING="-Os -O3 -D_USE_ALL_SYSTEM_FONTS" -DGUI=QT5 -DMAX_IMAGE_SCALE_MUL=2 -DDOC_DATA_COMPRESSION_LEVEL=3 -DDOC_BUFFER_SIZE=0x1400000 -DBIG_PAGE_MARGINS=1 ../coolreader

make V=1 -j4

strip -s cr3qt/cr3.exe
mv cr3qt/cr3.exe ./cr3-qt5.exe
curl -sS --upload-file cr3-qt5.exe https://transfer.sh/cr3-qt5.exe && echo -e '\n'

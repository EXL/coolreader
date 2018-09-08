FROM ubuntu:16.04
MAINTAINER EXL

SHELL ["/bin/bash", "-c"]

ADD . /usr/share/CoolReader3
WORKDIR /usr/share/CoolReader3

# Build Environment
RUN apt-get update -qq > /dev/null
RUN apt-get install -y software-properties-common python-software-properties > /dev/null
RUN add-apt-repository ppa:beineri/opt-qt-5.11.1-xenial -y > /dev/null
RUN apt-get update -qq > /dev/null
RUN apt-get -y install qt511base qt511tools qt511translations > /dev/null
RUN apt-get install -y gcc > /dev/null
RUN apt-get install -y g++ > /dev/null
RUN apt-get install -y build-essential > /dev/null
RUN apt-get install -y cmake > /dev/null
RUN apt-get install -y libpng-dev > /dev/null
RUN apt-get install -y libjpeg-dev > /dev/null
RUN apt-get install -y libfreetype6-dev > /dev/null
RUN apt-get install -y libfontconfig-dev > /dev/null
RUN apt-get install -y libgl1-mesa-dev > /dev/null
RUN apt-get install -y curl > /dev/null
RUN apt-get install -y p7zip-rar p7zip-full > /dev/null
RUN apt-get install -y git > /dev/null
RUN apt-get install -y mingw-w64 > /dev/null
RUN apt-get install -y qtchooser qttools5-dev-tools qt5-default > /dev/null

# Build AppImage for Linux, Qt 5, x86_64
RUN mkdir ../build-linux-qt5/
WORKDIR /usr/share/build-linux-qt5/
RUN source /opt/qt*/bin/qt*-env.sh && cmake -D GUI=QT5 -D CMAKE_BUILD_TYPE=Release -D MAX_IMAGE_SCALE_MUL=2 -D DOC_DATA_COMPRESSION_LEVEL=3 -D DOC_BUFFER_SIZE=0x1400000 -D CMAKE_INSTALL_PREFIX=/usr ../CoolReader3
RUN make -j$(nproc)
RUN make DESTDIR=/tmp/AppDir/ install

# Clean Source Dir
WORKDIR /usr/share/CoolReader3
RUN git clean -fdx && git checkout -- .

# Build App for MS Windows, Qt 4, x86
RUN mkdir ../build-windows-qt4/
WORKDIR /usr/share/build-windows-qt4/
RUN curl -LOJ "http://esxi.z-lab.me:666/~exl_lab/software/Qt4.8.7-static-win32-mingw-w64.7z"
RUN 7z x Qt4.8.7-static-win32-mingw-w64.7z
RUN mkdir -p /home/baat/
RUN ln -s /usr/share/build-windows-qt4/Qt4.8.7-static-win32-mingw-w64/ /home/baat/Qt4.8.7-static-win32-mingw-w64
RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS:STRING="-static -static-libgcc -static-libstdc++" -DQT_QMAKE_EXECUTABLE=./Qt4.8.7-static-win32-mingw-w64/bin/qmake -DCMAKE_TOOLCHAIN_FILE=./Toolchain-mingw64-i686.cmake -DUSE_QT_JPEG=1 -DUSE_QT_ZLIB=1 -DUSE_QT_PNG=1 -DCMAKE_CXX_FLAGS:STRING="-Os -O3 -D_USE_ALL_SYSTEM_FONTS" -DGUI=QT -DMAX_IMAGE_SCALE_MUL=2 -DDOC_DATA_COMPRESSION_LEVEL=3 -DDOC_BUFFER_SIZE=0x1400000 -DBIG_PAGE_MARGINS=1 -DCMAKE_INSTALL_PREFIX=/tmp/coolreader-win/ ../CoolReader3
RUN make -j$(nproc)
RUN make install

# Deploy All Builds
WORKDIR /tmp/
RUN strip -s AppDir/usr/bin/cr3
RUN curl -LOJ "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
RUN chmod a+x linuxdeployqt-continuous-x86_64.AppImage
RUN unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
RUN ./linuxdeployqt-continuous-x86_64.AppImage --appimage-extract
RUN curl -LOJ "http://esxi.z-lab.me:666/~exl_lab/software/qt_ru.qm"
RUN curl -LOJ "http://esxi.z-lab.me:666/~exl_lab/software/qtbase_ru.qm"
RUN mv qt_ru.qm qtbase_ru.qm AppDir/usr/share/cr3/i18n/
RUN source /opt/qt*/bin/qt*-env.sh && ./squashfs-root/AppRun AppDir/usr/share/applications/cr3.desktop -bundle-non-qt-libs
RUN source /opt/qt*/bin/qt*-env.sh && ./squashfs-root/AppRun AppDir/usr/share/applications/cr3.desktop -appimage
RUN i686-w64-mingw32-strip -s coolreader-win/cr3.exe
RUN 7z a -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=on coolreader-win-qt4.7z coolreader-win/
RUN curl -sS --upload-file ./CoolReader3-x86_64.AppImage https://transfer.sh/CoolReader3-x86_64.AppImage
RUN curl -sS --upload-file ./coolreader-win-qt4.7z https://transfer.sh/coolreader-win-qt4.7z

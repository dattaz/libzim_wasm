all: demo_file_api.js
build/lib/liblzma.so : 
	wget -N https://tukaani.org/xz/xz-5.2.4.tar.gz
	tar xf xz-5.2.4.tar.gz
	cd xz-5.2.4 ; ./autogen.sh
	cd xz-5.2.4 ; emconfigure ./configure --prefix=`pwd`/../build
	cd xz-5.2.4 ; emmake make 
	cd xz-5.2.4 ; emmake make install
	
build/lib/libz.a : 
	wget -N https://zlib.net/zlib-1.2.11.tar.gz
	tar xf zlib-1.2.11.tar.gz
	cd zlib-1.2.11 ; emconfigure ./configure --prefix=`pwd`/../build
	cd zlib-1.2.11 ; emmake make 
	cd zlib-1.2.11 ; emmake make install
	
build/lib/libzstd.a : 
	wget -N https://github.com/facebook/zstd/releases/download/v1.4.4/zstd-1.4.4.tar.gz
	tar xf zstd-1.4.4.tar.gz
	cd zstd-1.4.4/build/meson ; meson setup --cross-file=../../../emscripten-crosscompile.ini -Dbin_programs=false -Dbin_contrib=false -Dzlib=disabled -Dlzma=disabled -Dlz4=disabled --prefix=`pwd`/../../../build --libdir=lib builddir
	cd zstd-1.4.4/build/meson/builddir ; ninja
	cd zstd-1.4.4/build/meson/builddir ; ninja install
	
build/lib/libicudata.so : 
	wget -N https://github.com/unicode-org/icu/releases/download/release-63-2/icu4c-63_2-src.tgz
	tar xf icu4c-63_2-src.tgz
	# Quick and dirty way to make ICU handle Double-conversion, and to skip unnecessary compilation steps
	cd icu ; patch -p1 <../patch_icu_for_emscripten.patch
	cd icu/source ; emconfigure ./configure --prefix=`pwd`/../../build
	cd icu/source ; emmake make 
	cd icu/source ; emmake make install

build/lib/libxapian.a :
	wget -N https://oligarchy.co.uk/xapian/1.4.18/xapian-core-1.4.18.tar.xz
	tar xf xapian-core-1.4.18.tar.xz
	cd xapian-core-1.4.18; emconfigure ./configure --prefix=`pwd`/../build "CFLAGS=-I`pwd`/../build/include -L`pwd`/../build/lib" "CXXFLAGS=-I`pwd`/../build/include -L`pwd`/../build/lib" --disable-backend-remote --disable-shared
	cd xapian-core-1.4.18; emmake make "CFLAGS=-I`pwd`/../build/include -L`pwd`/../build/lib -std=c++11" "CXXFLAGS=-I`pwd`/../build/include -L`pwd`/../build/lib -std=c++11"
	cd xapian-core-1.4.18; emmake make install

build/lib/libzim.a : build/lib/liblzma.so build/lib/libz.a build/lib/libzstd.a build/lib/libicudata.so build/lib/libxapian.a
	wget -N --content-disposition https://github.com/openzim/libzim/archive/7.0.0.tar.gz
	tar xf libzim-7.0.0.tar.gz
	# It's no use trying to compile examples
	sed -i -e "s/^subdir('examples')//" libzim-7.0.0/meson.build
	cd libzim-7.0.0; PKG_CONFIG_PATH=/src/build/lib/pkgconfig meson --prefix=`pwd`/../build --cross-file=../emscripten-crosscompile.ini . build -DUSE_MMAP=false
	# Quick and dirty way to make libzim compilable with emscripten
	cd libzim-7.0.0 ; patch -p1 -F5 <../patch_libzim_for_emscripten.patch
	cd libzim-7.0.0; ninja -C build
	cd libzim-7.0.0; ninja -C build install

demo_file_api.js: build/lib/libzim.a demo_file_api.cpp prejs_file_api.js postjs_file_api.js
	em++ --bind demo_file_api.cpp build/lib/libzim.a -Ibuild/include -Lbuild/lib -lzstd -llzma -fdiagnostics-color=always -pipe -Wall -Winvalid-pch -Wnon-virtual-dtor -Werror -std=c++11 -O0 -g --pre-js prejs_file_api.js --post-js postjs_file_api.js -s DISABLE_EXCEPTION_CATCHING=0 -s "EXPORTED_RUNTIME_METHODS=['ALLOC_NORMAL','printErr','ALLOC_STACK','print']" -s DEMANGLE_SUPPORT=1 -s TOTAL_MEMORY=83886080 -s ALLOW_MEMORY_GROWTH=1 -lworkerfs.js

clean :
	rm -rf xz-*
	rm -rf zstd-*
	rm -rf zlib-*
	rm -rf xapian-core-*
	rm -rf icu*
	rm -rf libzim-*
	rm -rf build
	rm a.out.*

.PHONY : all clean

all: demo_file_api.js big_file_demo.js
build/lib/liblzma.so : 
	wget -N https://tukaani.org/xz/xz-5.2.4.tar.gz
	tar xf xz-5.2.4.tar.gz
	cd xz-5.2.4 ; ./autogen.sh
	cd xz-5.2.4 ; emconfigure ./configure --prefix=`pwd`/../build
	cd xz-5.2.4 ; emmake make 
	cd xz-5.2.4 ; emmake make install
	
build/lib/libz.a : 
	wget -N https://zlib.net/zlib-1.2.12.tar.gz
	tar xf zlib-1.2.12.tar.gz
	cd zlib-1.2.12 ; emconfigure ./configure --prefix=`pwd`/../build
	cd zlib-1.2.12 ; emmake make
	cd zlib-1.2.12 ; emmake make install
	
build/lib/libzstd.a : 
	wget -N https://github.com/facebook/zstd/releases/download/v1.4.4/zstd-1.4.4.tar.gz
	tar xf zstd-1.4.4.tar.gz
	cd zstd-1.4.4/build/meson ; meson setup --cross-file=../../../emscripten-crosscompile.ini -Dbin_programs=false -Dbin_contrib=false -Dzlib=disabled -Dlzma=disabled -Dlz4=disabled --prefix=`pwd`/../../../build --libdir=lib builddir
	cd zstd-1.4.4/build/meson/builddir ; ninja
	cd zstd-1.4.4/build/meson/builddir ; ninja install
	
build/lib/libicudata.so : 
	wget -N https://github.com/unicode-org/icu/releases/download/release-69-1/icu4c-69_1-src.tgz
	tar xf icu4c-69_1-src.tgz
	# It's no use trying to compile examples
	sed -i -e 's/^SUBDIRS =\(.*\)$$(DATASUBDIR) $$(EXTRA) $$(SAMPLE) $$(TEST)\(.*\)/SUBDIRS =\1\2/' icu/source/Makefile.in
	cd icu/source ; emconfigure ./configure --prefix=`pwd`/../../build
	cd icu/source ; emmake make 
	cd icu/source ; emmake make install

build/lib/libxapian.a : build/lib/libz.a
	wget -N https://oligarchy.co.uk/xapian/1.4.18/xapian-core-1.4.18.tar.xz
	tar xf xapian-core-1.4.18.tar.xz
        # Some options coming from https://github.com/xapian/xapian/tree/master/xapian-core/emscripten
	#cd xapian-core-1.4.18; emconfigure ./configure --prefix=`pwd`/../build "CFLAGS=-I`pwd`/../build/include -L`pwd`/../build/lib" "CXXFLAGS=-I`pwd`/../build/include -L`pwd`/../build/lib" CPPFLAGS='-DFLINTLOCK_USE_FLOCK' CXXFLAGS='-Oz -s USE_ZLIB=1 -fno-rtti' --disable-backend-honey --disable-backend-inmemory --disable-shared --disable-backend-remote
	cd xapian-core-1.4.18; emconfigure ./configure --prefix=`pwd`/../build "CFLAGS=-I`pwd`/../build/include -L`pwd`/../build/lib" "CXXFLAGS=-I`pwd`/../build/include -L`pwd`/../build/lib" --disable-shared --disable-backend-remote
	cd xapian-core-1.4.18; emmake make "CFLAGS=-I`pwd`/../build/include -L`pwd`/../build/lib -std=c++11" "CXXFLAGS=-I`pwd`/../build/include -L`pwd`/../build/lib -std=c++11"
	cd xapian-core-1.4.18; emmake make install

build/lib/libzim.a : build/lib/liblzma.so build/lib/libz.a build/lib/libzstd.a build/lib/libicudata.so build/lib/libxapian.a
	wget -N --content-disposition https://github.com/openzim/libzim/archive/7.2.2.tar.gz
	tar xf libzim-7.2.2.tar.gz
	# It's no use trying to compile examples
	sed -i -e "s/^subdir('examples')//" libzim-7.2.2/meson.build
	cd libzim-7.2.2; PKG_CONFIG_PATH=/src/build/lib/pkgconfig meson --prefix=`pwd`/../build --cross-file=../emscripten-crosscompile.ini . build -DUSE_MMAP=false
	cd libzim-7.2.2; ninja -C build
	cd libzim-7.2.2; ninja -C build install

demo_file_api.js: build/lib/libzim.a demo_file_api.cpp prejs_file_api.js postjs_file_api.js
	em++ --bind demo_file_api.cpp -I/src/build/include -L/src/build/lib -lzim -llzma -lzstd -lxapian -lz -licui18n -licuuc -licudata -lpthread -lm -fdiagnostics-color=always -pipe -Wall -Winvalid-pch -Wnon-virtual-dtor -Werror -std=c++11 -O0 -g --pre-js prejs_file_api.js --post-js postjs_file_api.js -s DISABLE_EXCEPTION_CATCHING=0 -s "EXPORTED_RUNTIME_METHODS=['ALLOC_NORMAL','printErr','ALLOC_STACK','print']" -s DEMANGLE_SUPPORT=1 -s TOTAL_MEMORY=83886080 -s ALLOW_MEMORY_GROWTH=1 -lworkerfs.js

big_file_demo.js:
	em++ --bind -std=c++11 -O0 --pre-js prejs_file_api_testbigfile.js --post-js postjs_file_api_testbigfile.js big_file_test.cpp -lworkerfs.js -o bigfile.js

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

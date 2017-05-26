all: demo.html demo_file_api.js
lzma : 
	wget https://tukaani.org/xz/xz-5.2.3.tar.gz
	tar zxvf xz-5.2.3.tar.gz
	cd xz-5.2.3 ; ./autogen.sh
	cd xz-5.2.3 ; emconfigure ./configure --prefix=`pwd`/../lzma
	cd xz-5.2.3 ; emmake make 
	cd xz-5.2.3 ; emmake make install

libzim : lzma
	git clone https://github.com/openzim/openzim
	cd openzim/zimlib; ./autogen.sh
	cd openzim/zimlib; emconfigure ./configure CFLAGS=-I`pwd`/../../lzma/include CXXFLAGS=-I`pwd`/../../lzma/include --prefix=`pwd`/../../libzim
	cd openzim/zimlib; emmake make CFLAGS=-I`pwd`/../../lzma/include CXXFLAGS=-I`pwd`/../../lzma/include
	cd openzim/zimlib; emmake make install



demo.html : lzma libzim demo.cpp meta.esperanto.stackexchange.com_eng_all_2017-05.zim 
	em++ demo.cpp -llzma -lzim -o demo.html -Ilzma/include -Llzma/lib -Ilibzim/include -Llibzim/lib --preload-file meta.esperanto.stackexchange.com_eng_all_2017-05.zim -s WASM=1

demo_file_api.js:lzma libzim demo_file_api.cpp prejs_file_api.js postjs_file_api.js
	em++ demo_file_api.cpp -llzma -lzim -o demo_file_api.js -Ilzma/include -Llzma/lib -Ilibzim/include -Llibzim/lib --pre-js prejs_file_api.js --post-js postjs_file_api.js -s WASM=1

clean :
	rm -rf demo.js demo.html demo.data demo.wasm demo_file_api.js demo_file_api.wasm 
	rm -rf openzim
	rm -rf xz-5.2.3 xz-5.2.3.tar.gz
clean_lib:
	rm -rf libzim lzma

.PHONY : all clean clean_lib

# Prototype of libzim in Webassembly

Demo at https://mossroy.github.io/libzim_wasm/

It uses WORKERFS as FS with emscripten and run in a web worker, file object is mount before run, and name is pass as argument.

## Steps to recompile
- Install emscripten : https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html
- Install dependencies necessary for compilation. On ubuntu 18.04, you need to activate universe repository and :
```
sudo apt install ninja-build meson pkg-config python3 autopoint libtool autoconf
sudo apt install zlib1g-dev libicu-dev libxapian-dev liblzma-dev
```
- activate emscripten environment variables with something like `source ./emsdk_env.sh`
- run `make`

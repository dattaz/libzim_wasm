# Prototype of libzim in Webassembly

Demo at https://mossroy.github.io/libzim_wasm/

It uses WORKERFS as FS with emscripten and run in a web worker, file object is mount before run, and name is passed as argument.

## Steps to recompile manually
- Install emscripten : https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html
- Install dependencies necessary for compilation. On ubuntu 18.04, you need to activate universe repository and :
```
sudo apt install ninja-build meson pkg-config python3 autopoint libtool autoconf
sudo apt install zlib1g-dev libicu-dev libxapian-dev liblzma-dev
```
- activate emscripten environment variables with something like `source ./emsdk_env.sh`
- run `make`

## Steps to recompile with Docker
While being at the root of this repository :
 - Build the Docker image with the provided Dockerfile (based on https://hub.docker.com/r/emscripten/emsdk , which is based on Debian) :
```
docker build -t "docker-emscripten-libzim:v2" ./docker
```
 - Run the build with :
```
docker run --rm -v $(pwd):/src -v /tmp/emscripten_cache/:/home/emscripten/.emscripten_cache -u $(id -u):$(id -g) -it docker-emscripten-libzim:v2 make
```

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
 - Build the Docker image with the provided Dockerfile (based on https://hub.docker.com/r/apiaryio/emcc/ , which is based on Debian) :
```
sudo docker build -t "docker-emscripten-libzim:v1" .
```
 - Run the build with :
```
sudo docker run --rm -v $(pwd):/src -v /tmp/emscripten_cache/:/root/.emscripten_cache -t docker-emscripten-libzim:v1 make
```
(please note that, in this case, the generated files will be owned by root after compiling, you might need to chown them afterwards)
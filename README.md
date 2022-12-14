# Prototype of libzim in WebAssembly (WASM)

This Repository provides the source code and utilities for compiling
the [ZIM File](https://wiki.openzim.org/wiki/ZIM_file_format) reader
[lbizim](https://wiki.openzim.org/wiki/Libzim) from C++ to
[WebAssembly](https://developer.mozilla.org/en-US/docs/WebAssembly)
(and
[ASM.js](https://developer.mozilla.org/en-US/docs/Games/Tools/asm.js)).

A prototype in HTML/JS, for testing the WASM version, is provided at
https://openzim.github.io/javascript-libzim/tests/prototype/. This
prototype uses WORKERFS as the Emscripten File System and runs in a
Web Worker. The file object is mounted before run, and the name is
passed as argument.

There is also an HTML/JS utility for testing the ability of Emscripten
File Systems to read large files (muliti-gigabyte) at
https://openzim.github.io/javascript-libzim/tests/test_large_file_access/.

[![CodeFactor](https://www.codefactor.io/repository/github/openzim/javascript-libzim/badge)](https://www.codefactor.io/repository/github/openzim/javascript-libzim)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

## Nightly and Release versions

WASM and ASM versions are built nightly from the binaries provided (nightly) by [kiwix-build](https://github.com/kiwix/kiwix-build). The artefacts are made
available at https://download.openzim.org/nightly/.

Released versions are published both in [Releases](https://github.com/openzim/javascript-libzim/releases) and at https://download.openzim.org/release/javascript-libzim/.

These versions are built with both the WORKERFS and the NODEFS [Emscripten File Systems](https://emscripten.org/docs/api_reference/Filesystem-API.html).
Please note that WORKERFS must be run in a Web Worker, and so the JavaScript glue (interface to the C++ code) is provided as a Worker. Messages are sent
to and received from the Worker via [`window.postMessage()`](https://developer.mozilla.org/en-US/docs/Web/API/Window/postMessage).

You can change the File Systems and other parameters in the provided [Makefile](https://github.com/openzim/javascript-libzim/blob/main/Makefile) in this Repository.
This recipe needs to be run in an Emscripten-configured system or a customized Emscripten container (see below).

## Steps to recompile manually

* Install Emscripten : https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html
* Install dependencies necessary for compilation. On ubuntu 18.04, you need to activate universe repository and:

```
sudo apt install ninja-build meson pkg-config python3 autopoint libtool autoconf
sudo apt install zlib1g-dev libicu-dev libxapian-dev liblzma-dev
```

* Activate emscripten environment variables with something like `source ./emsdk_env.sh`
* Run `make`.

## Steps to recompile from source with Docker

* Open a terminal at the root of this repository:
* Build the Docker image with the provided Dockerfile (based on https://hub.docker.com/r/emscripten/emsdk, which is based on Debian):

```
docker build -t "docker-emscripten-libzim:v3" ./docker
```

* Run the build with:

```
docker run --rm -v $(pwd):/src -v /tmp/emscripten_cache/:/home/emscripten/.emscripten_cache -u $(id -u):$(id -g) -it docker-emscripten-libzim:v3 make
```

## Licence

[GPLv3](https://www.gnu.org/licenses/gpl-3.0) or later, see
[LICENCE](LICENSE) for more details.

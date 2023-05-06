
# Esports platfrorm for Web and Mobile platforms

This platform is built using C because of its simplicity, efficiency, and speed.
It leverages GLFW, a lightweight and portable library that provides an easy-to-use API
for creating windows, managing OpenGL contexts, and handling events.

The project can be executed in a web environment thanks to its Emscripten build,
which enables the C codebase to be compiled into WebAssembly and render graphics onto an HTML canvas element.
Emscripten can translate OpenGL code into WebGL,
which allows the same OpenGL codebase to run on multiple platforms, including the web.

## Build

To compile the WebAssembly build of this project,
you only need Docker installed on your system,
as the entire build toolchain is fully containerized.

Simply run the following command in your terminal:

    docker-compose run wasm 'make clean build'

Web application files will be genrated in `./wasm/build/`.
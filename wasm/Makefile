#
# WASM build
#

build:
	mkdir -p ./build

	emcc ../src/esport.c \
		-o ./build/index.html \
		-s FULL_ES2=1 -s USE_GLFW=3 -s USE_WEBGL2=1 -O0 \
		-s ALLOW_MEMORY_GROWTH=1 -s GL_UNSAFE_OPTS=0 \
    --preload-file ../src/shaders/light-vert.glsl@src/shaders/light-vert.glsl \
    --preload-file ../src/shaders/light-frag.glsl@src/shaders/light-frag.glsl \
    --preload-file ../src/shaders/default-vert.glsl@src/shaders/default-vert.glsl \
    --preload-file ../src/shaders/default-frag.glsl@src/shaders/default-frag.glsl \
		--preload-file ../assets/ppm/05-01-wall.ppm@assets/ppm/05-01-wall.ppm

clean:
	rm -fr build/

.PHONY=clean

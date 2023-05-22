#
# WASM build
#

build:
	mkdir -p ./build

	emcc \
		../src/model.c ../src/camera.c ../src/mesh.c ../src/buffers.c ../src/ppm.c \
		../src/texture.c ../src/shader.c ../src/esport.c \
		-o ./build/index.html \
		-s FULL_ES2=1 -s USE_GLFW=3 -s USE_WEBGL2=1 -O0 \
		-s ALLOW_MEMORY_GROWTH=1 -s GL_UNSAFE_OPTS=0 \
		-s ASSERTIONS=1 -s SAFE_HEAP=1 \
    --preload-file ../src/shaders/light-vert.glsl@src/shaders/light-vert.glsl \
    --preload-file ../src/shaders/light-frag.glsl@src/shaders/light-frag.glsl \
    --preload-file ../src/shaders/default-vert.glsl@src/shaders/default-vert.glsl \
    --preload-file ../src/shaders/default-frag.glsl@src/shaders/default-frag.glsl \
		--preload-file ../assets/ppm/steve.ppm@assets/ppm/steve.ppm \
		--preload-file ../assets/ppm/test.ppm@assets/ppm/test.ppm \
		--preload-file ../assets/ppm/text.ppm@assets/ppm/text.ppm \
		--preload-file ../assets/ppm/mapped-skin.ppm@assets/ppm/mapped-skin.ppm \
		--preload-file ../assets/ppm/02-04-wall.ppm@assets/ppm/02-04-wall.ppm \
		--preload-file ../assets/ppm/04-12-wall.ppm@assets/ppm/04-12-wall.ppm \
		--preload-file ../assets/ppm/05-01-wall.ppm@assets/ppm/05-01-wall.ppm \
		--preload-file ../assets/ppm/06-01-metal.ppm@assets/ppm/06-01-metal.ppm \
		--preload-file ../assets/ppm/07-03-floor.ppm@assets/ppm/07-03-floor.ppm \
		--preload-file ../assets/ppm/08-01-wall.ppm@assets/ppm/08-01-wall.ppm \
		--preload-file ../assets/ppm/10-11-lava.ppm@assets/ppm/10-11-lava.ppm \
		--preload-file ../assets/ppm/15-03-box-top.ppm@assets/ppm/15-03-box-top.ppm \
		--preload-file ../assets/ppm/17-03-grass.ppm@assets/ppm/17-03-grass.ppm \
		--preload-file ../assets/ppm/18-17-leaf.ppm@assets/ppm/18-17-leaf.ppm \
		--preload-file ../assets/ppm/21-01-rusty.ppm@assets/ppm/21-01-rusty.ppm \
		--preload-file ../assets/ppm/30-03-top-block.ppm@assets/ppm/30-03-top-block.ppm \
		--preload-file ../assets/ppm/30-03-ground-with-grass-on-top.ppm@assets/ppm/30-03-ground-with-grass-on-top.ppm \
		--preload-file ../assets/ppm/31-03-ground.ppm@assets/ppm/31-03-ground.ppm \
		--preload-file ../assets/obj/luke.obj@assets/obj/luke.obj \
		--preload-file ../assets/obj/level-map.obj@assets/obj/level-map.obj \
		--preload-file ../assets/obj/cube.obj@assets/obj/cube.obj

clean:
	rm -fr build/

.PHONY=clean

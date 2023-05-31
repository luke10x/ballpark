#
# Desktop application build
#

ifeq ($(shell uname), Darwin)
	BREW_PREFIX    := $(shell brew --prefix)
	C_INCLUDE_PATH := -I $(BREW_PREFIX)/include/ -I $(BREW_PREFIX)/include/bullet/
	LIBRARY_PATH   := -L $(BREW_PREFIX)/lib
	LIBRARIES      := -lLinearMath -lBulletDynamics -lBulletCollision	\
		-lglfw -lglew -lstdc++ \
		-framework CoreVideo -framework OpenGL -framework GLUT \
		-framework IOKit -framework Cocoa -framework Carbon
else
	LIBRARIES := -lstdc++ -lGL -lGLEW -lglfw -lm -lcglm
endif

ARCH := $(shell uname -m)
OS := $(shell uname -s)

TARGET := esport-$(ARCH)-$(OS).app

$(TARGET):
	mkdir -p ./obj
	g++ -c src/physics.cpp -o ./obj/physics.o $(C_INCLUDE_PATH) $(LIBRARY_PATH) $(LIBRARIES)

	gcc -g \
		src/objloader.c \
		src/model.c src/camera.c src/mesh.c src/buffers.c src/ppm.c \
		src/texture.c src/shader.c src/esport.c \
		obj/physics.o \
		-o $(TARGET) \
		-DGL_SILENCE_DEPRECATION \
		$(C_INCLUDE_PATH) \
		$(LIBRARY_PATH) $(LIBRARIES)

clean:
	rm -f tmp/*.o
	rm -f *.out
	rm -f *.exe
	rm -f *.app
	rm -f *.elf
	rm -f obj/*.o
	rm -rf *.dSYM
	# rm -fr assets/ppm
	# rm -fr assets/obj

run: clean assets/obj assets/ppm $(TARGET)
	@echo "Press any key..."
	@dd bs=1 count=1 2>/dev/null
	./$(TARGET)

.PHONY=clean run

assets/ppm:
	mkdir -p assets/ppm
	mogrify -path assets/ppm -format ppm -fill "rgb(255,0,255)" -opaque "transparent" assets/png/*.png
	mogrify -path assets/ppm -format ppm assets/jpg/*.jpg
	# mogrify -path assets/ppm/alpha -format ppm -alpha extract assets/png/*.png

assets/obj:
	mkdir -p assets/obj
	blender -b assets/level-map.blend -o assets/obj/level-map.obj --python-expr \
		"import bpy; bpy.ops.export_scene.obj(filepath='assets/obj/level-map.obj', use_triangles=True)"
	blender -b assets/luke.blend -o assets/obj/luke.obj --python-expr \
		"import bpy; bpy.ops.export_scene.obj(filepath='assets/obj/luke.obj', use_triangles=True)"
	blender -b assets/cube.blend -o assets/obj/cube.obj --python-expr \
		"import bpy; bpy.ops.export_scene.obj(filepath='assets/obj/cube.obj', use_triangles=True)"
	blender -b assets/sky.blend -o assets/obj/sky.obj --python-expr \
		"import bpy; bpy.ops.export_scene.obj(filepath='assets/obj/sky.obj', use_triangles=True)"
